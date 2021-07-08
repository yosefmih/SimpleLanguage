import argparse
import os 
import logging
import subprocess  

#global variables 
verbose = False      
debug_mode = False 
root_dir = None             # where the root directory is 
working_dir = None          # where the compiled files are stored
selected_test_groups = None # which test groups to run

TEST_FILES_ROOTS = 'tests'

def pretty_print(text, mode='info'):
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
    global root_dir 
    if root_dir==None:
        # set root_dir to be SimpleCompiler 
        root_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) 
    os.chdir(root_dir) 

def execute_shell_cmds(cmd, check=True, env=True, timeout=None, capture_output=True):
    logging.info('command: ' + ' '.join(cmd)) 
    response = subprocess.run(
        cmd, env=env, text=True, capture_output=capture_output, timeout=timeout
    ) 
    return_code = response.return_code 
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
    specs = []
    illegal_regex = re.compile('line \d+:\d+: (unexpected|expecting)')
    tests_dir = os.path.join(os.path.abspath('tests'), 'scanner')
    if not os.path.exists(tests_dir):
        raise Exception('Scanner tests not available')
    input_dir = os.path.join(tests_dir, 'input')
    output_dir = os.path.join(tests_dir, 'output')
    for input_file in sorted(os.listdir(input_dir)):
        output_file = fin_name.rsplit('.')[0] + '.out'
        input_file_path = os.path.join(input_dir, input_file)
        output_file_path = os.path.join(output_dir, output_file) 

        with open(output_file_path) as fout:
            expected = fout.read().strip() 
        is_illegal = any([
            illegal_regex.search(line) is not None
            for line in expected.split('\n')
        ])

        spec = {
            'name': input_file, 
            'target': 'scan',
            'code': input_file_path 
        }
        if is_illegal:
            spec['error_expected'] = True 
        else:
            spec['match_expected'] = output_file_path 
        specs.append(spec)

    return specs 

def prepare_parser_test_specs:
    specs = []
    tests_dir = os.path.join(os.path.abspath('tests'), 'parser')
    if not os.path.exists(tests_dir):
        raise Exception('Parser tests not available')
    legal_dir = os.path.join(tests_dir, 'legal')
    illegal_dir = os.path.join(tests_dir, 'illegal')
    pre_specs = [(f, True) for f in sorted(os.listdir(legal_dir))]
    pre_specs.extend([(f, False) for f in sorted(os.listdir(legal_dir))])
    for input_file, legal in pre_specs:
        input_dir = legal_dir if legal else illegal_dir 
        input_file_path = os.path.join(input_dir, input_file) 

        spec = {
            'name': input_file,
            'target': 'parse',
            'code': input_file_path
        }
        if legal:
            spec['error_not_expected'] = True 
        else:
            spec['error_expected'] = True 
        specs.append(spec)
    
    return specs 

def prepare_semantics_test_specs:
    pass 

def prepare_codegen_test_specs:
    pass 

def prepare_optimizer_test_specs:
    pass 

test_groups_specs = [
    ('scanner', prepare_parser_test_specs)
    ('parser', prepare_parser_test_specs)
    ('semantics', prepare_semantics_test_specs)
    ('codegen', prepare_codegen_test_specs)
    ('optimizer', prepare_optimizer_test_specs)
]

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

def run_tests():
    for tg in selected_test_groups:
        tg_spec = next(
            spec for spec in test_groups_specs if spec[0] == tg
        )
        name = tg_spec[0] 
        prepare_specs_func = tg_spec[1]
        pretty_print(f'Running {name} tests...')
        specs = prepare_specs_func()
        passed 

#debug
#directory 
#verbose
#test_group
#file
def parse_args():
    parser = argparse.ArgumentParser(description='Running tests')
    test_group_names = [tg_spec[0] for tg_spec in test_groups_specs] 
    parser.add_argument(dest='test_groups', nargs='+', type=str, choices=test_group_names, help='groups of tests to run')
    parser.add_argument('-d', '--debug', dest='debug', action='store_const', const=True, default=False, help='run in debug mode, real time output')
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_const', const=True, default=False, help='print more detailed output')
    parser.add_argument('-D', '--directory', dest='dir', type='str', default=None, help='where executables are wrote, by default it is temp')
    parser.add_argument('-f', '--filter', type=str, default=None, help='run specific files')
    return parser.parse_args() 

if __name__ == '__main__':
    args = parse_args()

    adjust_root_dir() 

    selected_test_groups = args.test_group_names

    # if verbose is set, setup logging 
    if args.verbose:
        verbose = True 
        loggin.basicConfig(level=logging.DEBUG, format='%(message)s')
    
    if args.dir != None:
        working_dir = os.path.abspath(args.dir)
    
    architecture_check()
    run_tests() 
    
