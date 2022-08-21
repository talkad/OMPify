import os
import shutil

repos_dir = 'git_repos'
extentions = ['.c', '.cpp', '.f90', '.h', '.f', '.cc']
start_idx = len(os.path.join(os.getcwd(), repos_dir)) + 1

def copy_file(src, dst):
	src = src[start_idx:]
	dst = os.path.join(dst, src)
	dstfolder = os.path.dirname(dst)

	if not os.path.exists(dstfolder):
		os.makedirs(dstfolder)

	shutil.copy(os.path.join(repos_dir, src), dst)

def scan_dir(root_dir):

	result = {}

	for idx, (root, dirs, files) in enumerate(os.walk(root_dir)):
		for file_name in files:
			ext = os.path.splitext(file_name)[1].lower()
			
			if ext in extentions:
				with open(f'{root}/{file_name}', 'rb') as f:
					for line in f:
						l = line.lower()

						if (b'pragma ' in l and b' omp ' in l and b' for' in l) or (b'omp ' in l and b' do' in l):
							result[ext] = (result[ext] if ext in result else 0) + 1
							copy_file(f'{root}/{file_name}', 'repositories_openMP')

		if idx % 10**3 == 0:
			print(f'{idx // (10**3)}) {result}')

	return result

res = scan_dir("/home/talkad/Downloads/thesis/data_gathering_script/git_repos")
print(res)
