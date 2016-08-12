# Write a python script to call an existing C++ executable
# R. Sheehan 12 - 8 - 2016

# The following should help
#https://docs.python.org/2/library/subprocess.html
#http://stackoverflow.com/questions/1996518/retrieving-the-output-of-subprocess-call
#http://stackoverflow.com/questions/13222808/how-to-run-external-executable-using-python/13222809
#http://stackoverflow.com/questions/35021664/how-to-run-exe-file-with-command-from-python-shell-or-idle

import sys # use this to access system routines
import os # use this to move between directories
import re # use this to access regex capabilities
import glob # use this to list all files in a directory
import subprocess # use this to run executables

print os.getcwd()
exe_dir = "C:\\users\\robert.sheehan\\Programming\\C++\\Bessel\\Debug\\"
#os.chdir(exe_dir)
#print os.getcwd()

prog_name = "Bessel.exe "
arg_vals = "1 200 0 5"
#arg_vals = "3.2456"
args = exe_dir + prog_name + arg_vals

# shell=False is to be used as standard practice unless you know what you're doing!

output = subprocess.call(args, stdin=None, stdout=None, stderr=None, shell=False)
#Output from subprocess.call() should only be redirected to files
print output # this will print the 0 value returned by main

# use subprocess.check if you want to print the output from the program to the
# python shell
#print subprocess.check_output(args, shell=False)

