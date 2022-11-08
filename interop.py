import pickle
import os

def load(file_path):
    with open(file_path, 'rb') as f:
        return pickle.load(f)


def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


def scan_dir():
    count = 0

    for idx, (root, dirs, files) in enumerate(os.walk('/home/talkad/Downloads/thesis/data_gathering_script/cpp_loops')):
        for file_name in files:

            data = load(os.path.join(root, file_name))
            pragma = data.omp_pragma

            if 'interop' in data.textual_loop or (pragma is not None and 'interop' in pragma):
                log('interop.txt', f'{pragma}\n{data.textual_loop}\n==========')
                count += 1
            # if 'reduction' in pragma:
            #     log('reduction.txt', f'{pragma}\n{data.textual_loop}\n==========')

    print(count)

                
scan_dir()