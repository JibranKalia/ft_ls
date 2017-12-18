import subprocess
import os
import shutil
import unittest
import shlex

testdirectory = '/tmp/ls-test'

def buildEnv(directory=testdirectory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def cleanEnv(directory=testdirectory):
    shutil.rmtree(directory)

def setupEnv(command, directory=testdirectory):
    try:
        subprocess.run(shlex.split(command), cwd=directory)
    except PermissionError:
        pass
    except FileNotFoundError:
        buildEnv()

def mainLS(args, directory=testdirectory):
    allArgs = shlex.split('/bin/ls ' + args + ' ' + testdirectory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return (lsreturn.stdout.decode())
    
def testLS(args, directory=testdirectory):
    allArgs = shlex.split(os.getcwd() + '/ft_ls ' + args + ' ' + testdirectory)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return (lsreturn.stdout.decode())

class TestLSCompare(unittest.TestCase):
    maxDiff=None

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

    def test_07_test_opt_t_0(self):
        setupEnv("touch -t 201312101830.55 a")
        setupEnv("touch -t 201212101830.55 b")
        setupEnv("touch -t 201412101830.55 c")
        setupEnv("touch -t 201411221830.55 d")
        setupEnv("touch -t 201405212033.55 e")
        setupEnv("touch -t 201409221830.55 f")
        setupEnv("touch -t 202007221830.55 g")
        setupEnv("touch -t 300012101830.55 h")
        args = '-1t'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_07_test_opt_t_6(self):
        setupEnv("touch C")
        setupEnv("touch -t 201212101830.55 c")
        setupEnv("mkdir -p sbox sbox1")
        setupEnv("touch -t 201312101830.55 B")
        setupEnv("touch -t 201312101830.55 a")
        args = "-1t a C B sbox sbox1"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_07_test_opt_t_7(self):
        setupEnv("touch C")
        setupEnv("touch -t 201212101830.55 c")
        setupEnv("mkdir -p sbox sbox1")
        setupEnv("touch -t 201312101830.55 B")
        setupEnv("touch -t 201312101830.55 a")
        args = "-1t"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    @unittest.skip("Not handling symbolic paths")
    def test_sys_00_test_user_bin(self):
        args = '-lR /usr/bin'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

def uniqueEnv():
    print("Env made")
    currentdir = "/Users/jibrankalia/project_ls/my_ls/test"
    buildEnv(currentdir)
    setupEnv("touch C", currentdir)
    setupEnv("touch -t 201212101830.55 c", currentdir)
    setupEnv("mkdir -p sbox sbox1", currentdir)
    setupEnv("touch -t 201312101830.55 B", currentdir)
    setupEnv("touch -t 201312101830.55 a", currentdir)

if __name__ == '__main__':
    #uniqueEnv()
    unittest.main()