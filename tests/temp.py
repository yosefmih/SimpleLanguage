import os 

print(__file__)
print(os.path.realpath(__file__))
print(os.path.dirname(os.path.realpath(__file__)))
print(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
print(os.path.abspath('..'))