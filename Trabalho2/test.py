import os, sys
from subprocess import PIPE, Popen


def cmdline(command):
    process = Popen(command, stdout=PIPE, shell=True, executable='/bin/bash', stderr=PIPE)
    return process.communicate()[0]

def unit_test(folder):
    print ("Starting unit tests...")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder)
    num_tests = 0
    num_failed_tests = 0
    
    files = [f for f in os.listdir(path) if f.endswith('.mmg')]

    for f in files:
        print ("Teste case:"), f,
        num_tests += 1

        command = "./out < tests/%s" %(f)
        output = cmdline(command)

        if "error:" not in output:
            print ("OK")
        else:
            print ("FAILED")
            print (output)
            num_failed_tests += 1

    print ("Total tests:"),  num_tests
    print ("Failed tests:"), num_failed_tests

def unit_test_diff(folder):
    print ("Starting unit tests...")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder)
    num_tests = 0
    num_failed_tests = 0

    directories = [directory for directory in os.listdir(path) if directory.startswith("test_")]
    for directory in directories:
        print ("Teste case:"), directory,
        num_tests += 1

        command = "diff <(./out < %s/%s/%s.mmg) %s/%s/%s.result" %(folder, directory, directory, folder, directory, directory)
        output = cmdline(command)

        if not output:
            print ("OK")
        else:
            print ("FAILED")
            print (output)
            num_failed_tests += 1

    print ("Total tests:"),  num_tests
    print ("Failed tests:"), num_failed_tests


def save_results(folder):
    print ("Saving tests results...")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder)

    for directory in os.listdir(path):
        command = "./out < %s/%s/%s.mmg > %s/%s/%s.result" %(folder, directory,  directory, folder, directory, directory)
        cmdline(command)


if sys.argv[1] == "saveresults":
    save_results(sys.argv[2])
elif sys.argv[1] == "unittest":
    unit_test(sys.argv[2])
elif sys.argv[1] == "unittest_diff":
    unit_test_diff(sys.argv[2])


    