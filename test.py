import subprocess
import os
import shutil
import unittest
import shlex

testdirectory = '/Users/jibrankalia/ls-test'

def buildEnv():
    if not os.path.exists(testdirectory):
        os.makedirs(testdirectory)

def cleanEnv():
    shutil.rmtree(testdirectory)

def refreshEnv():
    cleanEnv(testdirectory)
    buildEnv(testdirectory)

def setupEnv(command):
    try:
        subprocess.run(shlex.split(command), cwd=testdirectory)
    except PermissionError:
        pass
    except FileNotFoundError:
        buildEnv()

def mainLS(args):
    allArgs = shlex.split('/bin/ls ' + args + ' ' + testdirectory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE)
    return (lsreturn.stdout.decode())
    
def testLS(args):
    allArgs = shlex.split(os.getcwd() + '/ft_ls ' + args + ' ' + testdirectory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE)
    return (lsreturn.stdout.decode())

class TestLSCompare(unittest.TestCase):

    def setUp(self):
        buildEnv()

    def tearDown(self):
        cleanEnv()

    def testSimple(self):
        setupEnv('touch test')
        args = '-1'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def testSimple2(self):
        setupEnv('touch test')
        args = '-lr'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def testSimple3(self):
        setupEnv('mkdir - dir')
        args = '-lr'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

if __name__ == '__main__':
    unittest.main()