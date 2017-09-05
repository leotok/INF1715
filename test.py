import os, sys
from subprocess import PIPE, Popen


def cmdline(command):
    process = Popen(command, stdout=PIPE, shell=True, executable='/bin/bash', stderr=PIPE)
    return process.communicate()[0]

def unit_test():
    print ("Starting unit tests...")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tests")
    num_tests = 0
    num_failed_tests = 0
    
    directories = [directory for directory in os.listdir(path) if directory.startswith("test_")]

    for directory in directories:
        print ("Teste case:"), directory,
        num_tests += 1

        command = "diff <(./main < tests/%s/%s.mmg) tests/%s/%s.result" %(directory, directory, directory, directory)
        output = cmdline(command)

        if not output:
            print ("OK")
        else:
            print ("FAILED")
            print (output)
            num_failed_tests += 1

    print ("Total tests:"),  num_tests
    print ("Failed tests:"), num_failed_tests

def save_results():
    print ("Saving tests results...")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tests")

    for directory in os.listdir(path):
        command = "./main < tests/%s/%s.mmg > tests/%s/%s.result" %(directory, directory, directory, directory)
        cmdline(command)

if sys.argv[1] == "saveresults":
    save_results()
elif sys.argv[1] == "unittest":
    unit_test()


    