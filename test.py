import subprocess
import os
import shutil
import unittest
import shlex

# testdirectory = '/Users/jibrankalia/tmp/ls-test'
# testdirectory = '/tmp/ls-test'
testdirectory = '~/tmp/ls-test'

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

def mainLS(args):
    allArgs = shlex.split('/bin/ls ' + args)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=testdirectory)
    return (lsreturn.stdout.decode())
    
def testLS(args, directory=testdirectory):
    allArgs = shlex.split(os.getcwd() + '/ft_ls ' + args)
    lsreturn = subprocess.run(allArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=testdirectory)
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

    def testSimple3(self):
        setupEnv('mkdir - dir')
        args = '-lr'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def testSimple2(self):
        setupEnv('touch test')
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

    def test_08_test_opt_l_4(self):
        setupEnv("mkdir -p dir/.hdir")
        setupEnv("touch dir/.hdir/file")
        args = "-la dir"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_11_test_single_file_1(self):
        setupEnv("touch aaa")
        args = "-l aaa"
        expected = mainLS(args)
        mine = testLS(args)
        self.assertEqual(mine, expected)

    def test_13_test_hyphen_hard_1(self): 
        setupEnv("touch - file")
        args = "-1"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_13_test_hyphen_hard_2(self): 
        setupEnv("touch - file")
        args = "-1 -"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_13_test_hyphen_hard_3(self): 
        setupEnv("touch - file")
        args = "-1 --"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_21_test_symlink_1(self): 
        setupEnv("mkdir a")
        setupEnv("ln -s a b")
        setupEnv("rm -rf a")
        args = "-1 b"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_21_test_symlink_2(self):
        setupEnv("mkdir mydir")
        setupEnv("ln -s mydir symdir")
        setupEnv("touch mydir/file1 mydir/file1 mydir/file2 mydir/file3 mydir/file4 mydir/file5 ")
        args = "-1 symdir"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_22_test_no_username(self):
        args = "-l /usr/local/bin/node"
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    def test_24_test_multiple_files(self):
        setupEnv("touch a b C D")
        args = "-1 ./ ."
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    @unittest.skip("Dev Null Output is annoying")
    def test_08_test_opt_l_5(self):
        args = "-l"
        setupEnv("touch .a")
        setupEnv("dd bs=2 count=14450 if=/dev/random of=.a  >/dev/null 2>&1")
        setupEnv("ln -s .a b")
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

    @unittest.skip("-r-sr-xr-x Random s")
    def test_sys_00_test_user_bin(self):
        args = '-lR /usr/bin'
        expected = mainLS(args)
        self.assertEqual(testLS(args), expected)

def uniqueEnv():
    currentdir = "/Users/jibrankalia/project_ls/test"
    buildEnv(currentdir)
    setupEnv("touch - file", currentdir)

if __name__ == '__main__':
    uniqueEnv()
    unittest.main()
