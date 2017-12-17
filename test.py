import subprocess
import os
import shutil
import unittest
import shlex

td = '/Users/jibrankalia/ls-test'

def buildEnv(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def cleanEnv(directory):
    shutil.rmtree(directory)

def refreshEnv(directory):
    cleanEnv(directory)
    buildEnv(directory)

def setupEnv(command):
    try:
        subprocess.run(shlex.split(command))
    except PermissionError:
        pass
    except FileNotFoundError:
        buildEnv(td)

def mainLS(directory, args):
    allArgs = shlex.split('/bin/ls ' + args + ' ' + directory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE)
    return (lsreturn.stdout.decode())
    
def testLS(directory, args):
    allArgs = shlex.split('/Users/jibrankalia/ls/ft_ls/ft_ls ' + args + ' ' + directory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE)
    return (lsreturn.stdout.decode())

class TestLSCompare(unittest.TestCase):

    def setUp(self):
        setupEnv(td)

    def tearDown(self):
        cleanEnv(td)

    def testSimple(self):
        setupEnv('touch ' + td + '/test')
        args = '-1'
        expected = mainLS(td, args)
        self.assertEqual(testLS(td, args), expected)

    def testSimple2(self):
        setupEnv('touch ' + td + '/test')
        args = '-lr'
        expected = mainLS(td, args)
        self.assertEqual(testLS(td, args), expected)

if __name__ == '__main__':
    unittest.main()