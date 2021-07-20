#!/usr/bin/env python3

import argparse
import os 
import logging
import subprocess  
import queue 
import threading 
import fnmatch
import re

#global variables 
verbose = False      
debug_mode = False 
root_dir = None             # where the root directory is 
working_dir = None          # where the compiled files are stored
selected_test_groups = None # which test groups to run
test_filter = None
num_jobs = 1

TEST_FILES_ROOTS = 'tests'


def pretty_print(text, mode='info'):
    """
    Prints text formatted according to mode.
    If mode is 'info', printed with the default python print 
    If mode is 'failure', printed in red 
    If mode is 'warning', printed in yellow 
    If mode is 'success', printed in green
    
    Keyword arguments:
    text -- a str text to be printed 
    mode -- one of 'info', 'failure', 'warning' or 'success'
    """
    if mode == 'info':
        print(text)
    elif mode == 'failure':
        # print in red
        print('\033[91m' + text + '\033[0m' )
    elif mode == 'warning':
        # print in yellow 
        print('\033[93m' + text + '\033[0m')
    elif mode == 'success':
        # print in green 
        print('\033[92m' + text + '\033[0m') 


def adjust_root_dir():
    """
    Makes sure the working directory is the directory two steps before this file,
    which should be the root
    """
    global root_dir 
    if root_dir==None:
        # set root_dir to be SimpleCompiler 
        root_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) 
    os.chdir(root_dir) 


def execute_shell_cmds(cmd, check=True, env=None, timeout=None, capture_output=True):
    """
    Executes a bash command as specified in cmd
    """
    logging.info('command: ' + ' '.join(cmd))
    response = subprocess.run(
        cmd, env=env, text=True, capture_output=capture_output, timeout=timeout
    ) 
    return_code = response.returncode 
    logging.debug(f'return code: {return_code}')
    if capture_output:
        stdout = response.stdout.strip() 
        stderr = response.stderr.strip() 
        if stdout != '':
            logging.debug('stdout:')
            logging.debug(stdout) 
        if stderr != '':
            logging.debug('stderr:')
            logging.debug(stderr) 
    else:
        stdout = None 
        stderr = None 
    logging.debug('')

    if check and return_code != 0:
        raise Exception(f'Shell call returned {return_code}') \

    return return_code, stdout, stderr 


def prepare_scanner_test_specs():
    """
    Creates a spec for each test in the scanner stage.
    Each spec is a dictionary containing:
        spec['input_file'] -- name of input file 
        spec['input_file_path'] -- path to input file 
        spec['stage] -- 'scan'
    And either of the following two 
        spec['match_this'] -- a path to an output file 
        spec['error_expected'] -- True

    Returns: a list of specs, one for each test
    """
    specs = []
    tests_dir = os.path.join(os.path.abspath('tests'), 'scanner')
    if not os.path.exists(tests_dir):
        raise Exception('Scanner tests not available')
    # get the paths for the input and output folders of the scanner tests
    input_dir = os.path.join(tests_dir, 'input')
    output_dir = os.path.join(tests_dir, 'output')
    for input_file in sorted(os.listdir(input_dir)):
        #for each input file, find the corresponding output file
        output_file = input_file.rsplit('.')[0] + '.out'
        input_file_path = os.path.join(input_dir, input_file)
        output_file_path = os.path.join(output_dir, output_file) 

        # regex to match output files for input files that should through an error
        illegal_regex = re.compile('line \d+:\d+: (unexpected|expecting)')
        with open(output_file_path) as fout:
            expected = fout.read().strip() 
        is_illegal = any([
            illegal_regex.search(line) is not None
            for line in expected.split('\n')
        ])

        spec = {
            'input_file': input_file, 
            'stage': 'scan',
            'input_file_path': input_file_path 
        }
        if is_illegal:
            # if the regex from above matched, the scanner should output an error
            spec['error_expected'] = True 
        else:
            # otherwise it should match the content of the file at the output path
            spec['match_this'] = output_file_path 
        specs.append(spec)

    return specs 


def prepare_parser_test_specs():
    specs = []
    tests_dir = os.path.join(os.path.abspath('tests'), 'parser')
    if not os.path.exists(tests_dir):
        raise Exception('Parser tests not available')
    legal_dir = os.path.join(tests_dir, 'legal')
    illegal_dir = os.path.join(tests_dir, 'illegal')
    pre_specs = [(f, True) for f in sorted(os.listdir(legal_dir))]
    pre_specs.extend([(f, False) for f in sorted(os.listdir(illegal_dir))])
    for input_file, legal in pre_specs:
        input_dir = legal_dir if legal else illegal_dir 
        input_file_path = os.path.join(input_dir, input_file) 

        spec = {
            'input_file': input_file, 
            'stage': 'parse',
            'input_file_path': input_file_path 
        }
        if legal:
            spec['error_not_expected'] = True 
        else:
            spec['error_expected'] = True 
        specs.append(spec)
    
    return specs 


def prepare_semantics_test_specs():
    pass 


def prepare_codegen_test_specs():
    pass 


def prepare_optimizer_test_specs():
    pass 


def build():
    return_code, _, _ = execute_shell_cmds([
        'make'
    ], check=False)
    return return_code == 0


def architecture_check():
    arch_check_regex = re.compile('^Target: x86_64\S+linux-gnu')
    _, _, output = execute_shell_cmds(
        cmd = ['gcc', '-v']
    )
    # check if the output from the shell has a line that matches the regex from 
    # above 
    is_good_arch = any([
        arch_check_regex.search(line) is not None 
        for line in output.split('\n')
    ])
    if not is_good_arch:
        # arch is not x86_64
        pretty_print('WARNING: GCC architecture is not x86_64-linux-gnu', 'warning')


def expected_output(spec):
    with open(spec['match_this']) as fout:
        expected = fout.read() 

    if verbose:
        logging.debug('expected:')
        logging.debug(expected) 
    return expected 


def check_compiler_output(spec, return_code, compiler_output):
    input_file = spec['input_file']

    if 'error_expected' in spec and spec['error_expected']:
        if return_code == 0:
            pretty_print(f'FAILED TO REJECT ILLEGAL FILE -- {input_file}', 'failure')
            return False 
        else:
            pretty_print(f'CORRECTLY REJECTED -- {input_file}', 'success')
            return True 

    if return_code != 0:
        pretty_print(f'FAILED TO COMPILE -- {input_file}', 'failure')
        return False 

    if 'error_not_expected' in spec and spec['error_not_expected']:
        pretty_print(f'PASSED -- {input_file}', 'success')
        return True

    if 'match_this' in spec:
        expected = expected_output(spec)
        if compiler_output.strip() != expected.strip():
            pretty_print(f'WRONG OUTPUT -- {input_file}', 'failure')
            return False 
        else:
            pretty_print(f'PASSED -- {input_file}', 'success')
            return True 

    return None 

    
def compile(spec):
    input_file_path = spec['input_file_path']
    stage = spec['stage']

    if verbose:
        with open(input_file_path) as fin:
            code = fin.read() 
        logging.debug('input:')
        logging.debug(code) 
    
    cmd = [
        './main',
        f'--stage={stage}',
    ]

    if debug_mode:
        cmd += ['--debug']
    
    cmd += [f'--infile={input_file_path}']
    return_code, compiler_output, _ = execute_shell_cmds(
        cmd, check=False, capture_output=True
    )
    return return_code, compiler_output


def run_test(spec):
    input_file = spec['input_file']
    try:
        return_code, compiler_output = compile(spec)
        result = check_compiler_output(spec, return_code, compiler_output)
        if result is not None:
            return result 
        raise Exception('invalid test spec')
    except subprocess.TimeoutExpired:
        pretty_print(f'TIME LIMIT EXCEEDED DURING COMPILATION -- {input_file}', 'failure')
        return False 


def run_tests(specs):
    if not build():
        pretty_print('BUILD ERROR', 'failure')
        return 0 
    
    passed = 0
    q = queue.Queue() 
    l = threading.Lock() 
    def do_tests_from_queue():
        nonlocal q, l, passed 
        while True:
            try:
                spec = q.get_nowait() 
                if run_test(spec):
                    with l:
                        passed += 1
            except queue.Empty:
                break 
            q.task_done()
        
    for spec in specs:
        skip_test = (
            test_filter is not None and 
            not fnmatch.fnmatch(spec['input_file'], test_filter) and 
            not fnmatch.fnmatch(spec['input_file'].split('.')[0], test_filter) 
        )
        if not skip_test:
            q.put(spec) 
        
    if num_jobs > 1:
        for i in range(num_jobs):
            threading.Thread(target=do_tests_from_queue, daemon=True).start() 
    else:
        do_tests_from_queue()     
    q.join() 

    return passed


def run_selected_tests():
    """
    Runs the tests in the groups selected by the user
    """
    for tg in selected_test_groups:
        tg_spec = next(
            spec for spec in test_groups_specs if spec[0] == tg
        )
        group_name = tg_spec[0] 
        pretty_print(f'Running {group_name} tests...', 'info')
        # get the function responsible for preparing the individual tests in this group
        prepare_specs_func = tg_spec[1]
        specs = prepare_specs_func()
        passed = run_tests(specs) 
        pretty_print(f'PASSSED {passed}/{len(specs)} tests', 'info')

test_groups_specs = [
    ('scanner', prepare_scanner_test_specs),
    ('parser', prepare_parser_test_specs),
    ('semantics', prepare_semantics_test_specs),
    ('codegen', prepare_codegen_test_specs),
    ('optimizer', prepare_optimizer_test_specs)
]

#debug
#directory 
#verbose
#test_group
#file
def parse_args():
    parser = argparse.ArgumentParser(description='Running tests')
    test_group_names = [tg_spec[0] for tg_spec in test_groups_specs] 
    parser.add_argument(dest='test_group_names', nargs='+', type=str, choices=test_group_names, help='groups of tests to run')
    parser.add_argument('-d', '--debug', dest='debug', action='store_const', const=True, default=False, help='run in debug mode, real time output')
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_const', const=True, default=False, help='print more detailed output')
    parser.add_argument('-D', '--directory', dest='dir', type=str, default=None, help='where executables are wrote, by default it is temp')
    parser.add_argument('-f', '--filter', type=str, default=None, help='run specific files')
    parser.add_argument('-j', '--jobs', dest='jobs', type=int, default=1, help='number of threads for testing in parallel')
    return parser.parse_args() 


if __name__ == '__main__':
    args = parse_args()
    adjust_root_dir() 

    selected_test_groups = args.test_group_names
    test_filter = args.filter 
    debug_mode = args.debug 
    num_jobs = args.jobs

    # if verbose is set, setup logging 
    if args.verbose or debug_mode:
        verbose = True 
        logging.basicConfig(level=logging.DEBUG, format='%(message)s')
    
    if args.dir != None:
        working_dir = os.path.abspath(args.dir)
    
    architecture_check()
    run_selected_tests() 
    
