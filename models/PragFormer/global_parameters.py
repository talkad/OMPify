
import sys
sys.path.append("..")
# from ForPragmaExtractor.global_parameters import *
KEY_OPENMP = "pragma"
KEY_PICKLE = "code_pickle"
KEY_CODE = "code"
KEY_ID = 'id'
CUT_LARGER_THAN = 50


class Data:
    def __init__(self):
        self.train = []#train_text
        self.train_labels = []#train_labels
        self.val = []#val_text
        self.val_labels = []#val_labels
        self.test = []# test_text
        self.test_labels = []#test_labels
        self.train_ids = []
        self.val_ids = []
        self.test_ids = []


from pycparser import parse_file, c_ast, c_generator


class PragmaForTuple:
    """
    Class that holds 3 parameters.
    1) The Pragma c_ast node
    2) The For c_ast node that corresponds with the pragma
    3) Inner c_ast nodes that are inside "2" - function calls, variables, etc...
    """
    #def __init__(self, pragma_node: c_ast.Pragma, node: c_ast.For):
    def __init__(self, pragma_node, node):
        self.pragma = pragma_node
        self.for_node = node
        self.inner_nodes = []

    def set_inner_nodes(self, inner):
        self.inner_nodes = inner

    def has_openmp(self):
        coord = "%s" % self.pragma.coord
        if coord == "None":
            return False
        return True

    def get_string_data(self):
        generator = c_generator.CGenerator()
        code_data = generator.visit(self.for_node)
        for n in self.inner_nodes:
            code_data = code_data + "\n" + generator.visit(n)

        return self.pragma.string, code_data

    def get_coord(self):
        coord = "%s" % self.pragma.coord
        if coord == "None":
            coord = "%s" % self.for_node.coord
        return coord

class PragmaForTupleRegular(PragmaForTuple):
    """
    Class that holds 3 parameters.
    1) The Pragma c_ast node
    2) The For c_ast node that corresponds with the pragma
    3) Inner c_ast nodes that are inside "2" - function calls, variables, etc...
    """
    def __init__(self, Pragma_str, for_str):
        self.pragma = Pragma_str
        self.for_node = for_str
        self.inner_nodes = []

    def set_inner_nodes(self, inner):
        # self.inner_nodes = inner
        print("TRYING TO SET INNER NODES TO REGULAR PRAGMA - BAD!!")
        exit(1)
        pass

    def get_string_data(self):

        return self.pragma, self.for_node


###################################################### GLOBAL PRAMETERS #######################################3
MAX_RATIO_OMP = 20
PRAGMA_FILES = "/home/reemh/CLPP/CLPP/ForPragmaExtractor/pragma_files.txt"
FAKE_HEADER_PATH = "/home/reemh/CLPP/fake_headers"
WITH_NO_PRAGMA = True
GOOD_FILE = "/home/reemh/CLPP/CLPP/ForPragmaExtractor/success_file.txt"
BAD_FILE = "/home/reemh/CLPP/CLPP/ForPragmaExtractor/failed_file.txt"
LOG_FILE = ""
DB_PATH = ""
JSON_PATH = ""
DEBUG = False
EXCLUDE_FILES = ["/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/FG2_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/FGT_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/andotp_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/c3_fmt.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/wow_srp_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/clipperz_srp_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/pgpsda_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/electrum_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/known_hosts_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/ssh_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_7z_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_DES_bs_f_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_DES_bs_b_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_DES_bs_h_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_electrum_modern_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/opencl_rar_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/rawmd5u_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/rsvp_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/sapG_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/siemens-s7_fmt_plug.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/john/src/yescrypt/yescrypt-opt.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/openwall/php_mt_seed/php_mt_seed.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/klueska/libgomp-parlib/libgomp/testsuite/libgomp.c/omp_workshare3.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/klueska/libgomp-parlib/libgomp/testsuite/libgomp.c/pr35130.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/vazexqi/OpenMPMicroBenchmarks/ParallelFor_vs_ParallelTask/BlackScholes/blackscholes_for.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/berezhko/openmp/openmp.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/xiaodao90/wrapper/OpenMP/OMP_wrapper.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/ParRes/Kernels/MPIOPENMP/Synch_p2p/p2p.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/clang-omp/clang/test/OpenMP/distribute_parallel_for_misc_messages.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/clang-omp/clang/test/OpenMP/distribute_parallel_for_simd_misc_messages.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/clang-omp/clang/test/OpenMP/parallel_for_misc_messages.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/chunhualiao/gomp-4.4.4/testsuite/libgomp.c/pr35130.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/utkarshdalal/MatrixMultiply/sgemm-openmp.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/atmnpatel/openmp-rpc-offloading/clang/test/OpenMP/parallel_master_taskloop_misc_messages.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/atmnpatel/openmp-rpc-offloading/clang/test/OpenMP/parallel_master_taskloop_simd_misc_messages.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/atmnpatel/openmp-rpc-offloading/openmp/runtime/test/lock/omp_init_lock.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/atmnpatel/openmp-rpc-offloading/polly/lib/External/ppcg/cpu.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench/DRB052-indirectaccesssharebase-orig-no.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench/DRB085-threadprivate-orig-no.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench/DRB091-threadprivate2-orig-no.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench/DRB104-nowait-barrier-orig-no.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench/DRB110-ordered-orig-no.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Cetus/rodinia_3.1/openmp/heartwall/AVI/avilib.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/adammaj1/Mandelbrot-set-with-blended-gradients/src/d.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/SciPAL/SciPAL/include/numerics/propagation_kernels.cu.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/SciPAL/SciPAL/src/cuda/scipal_kernels.cu.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/clang-ykt/openmp/libomptarget/omptests/nt-target-map-ptr/test.c",
                 "/home/reemh/CLPP/github-clone-all/repos_final/OpenCMISS-Dependencies/opari2/test/data/test8.c"
                 ]
EXCLUDE_FOLDERS = ["/home/reemh/CLPP/github-clone-all/repos_final/clang-omp/clang/test/OpenMP/", "/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/Autopar/dataracebench"
                   ,"/home/reemh/CLPP/github-clone-all/repos_final/SebastianCojocariu/Kernels-Parallelization-Techniques-Serial-MPI-OpenMP-Pthreads-Hybrid-/APP/hibrid/"]
NESTED_FILES = ["/home/reemh/CLPP/github-clone-all/repos_final/LLNL/AutoParBench/benchmarks/baseline/dataracebench/DRB041-3mm-parallel-no.c",
                "/home/reemh/CLPP/github-clone-all/repos_final/rose-compiler/rose/projects/CodeThorn/src/tests/ompcfg/Par-14-NestedFollowingNestedPar.c"]
MAX_NUM_NON_OPENMP = 100
