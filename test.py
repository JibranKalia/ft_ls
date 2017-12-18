import subprocess
import os
import shutil
import unittest
import shlex

testdirectory = '/tmp/ls-test'

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

    @unittest.skip("Not handling symbolic paths")
    def test_sys_00_test_user_bin(self):
        # self.maxDiff=None
        args = '-lR /usr/bin'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_07_test_opt_t(self):
        self.maxDiff=None
        setupEnv("touch -t 201312101830.55 a")
        setupEnv("touch -t 201212101830.55 b")
        setupEnv("touch -t 201412101830.55 c")
        setupEnv("touch -t 201411221830.55 d")
        setupEnv("touch -t 201405212033.55 e")
        setupEnv("touch -t 201409221830.55 f")
        setupEnv("touch -t 202007221830.55 g")
        setupEnv("touch -t 300012101830.55 h")
        args = '-lTt'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)
if __name__ == '__main__':
    unittest.main()
