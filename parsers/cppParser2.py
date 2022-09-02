import clang.cindex as cl
cl.Config.set_library_file("/usr/lib64/llvm/libclang.so")

index = cl.Index.create()
# # tu = index.parse("/home/talkad/Downloads/thesis/data_gathering_script/asd/par_omp_sort.cpp")
# # node = tu.cursor
