import matplotlib.pyplot as plt
import os


#exts = ['.c', '.f', '.f77', '.f90', '.f95', '.f03', '.cc', '.cpp', '.cxx', '.h']

#def scan_dir(root_dir):

#	result = {}
#	for _, _, files in os.walk(root_dir):
#		for f in files:
#			ext = os.path.splitext(f)[1].lower()
#			
#			result[ext] = (result[ext] if ext in result else 0) + 1
#	
#	return result

#extensions = scan_dir("/home/talkad/Downloads/thesis/data_gathering_script/git_repos")
#print(extensions)

#extensions = {'.c': 328748, '.f': 13099, '.f77': 8, '.f90': 40685, '.f95': 871, '.f03': 2320, '.cc': 29875, '.cpp': 278584, '.cxx': 2904, '.h': 280344}

dist = {'.cpp': 134883, '.c': 68849, '.f90': 15209, '.h': 2696, '.cc': 1172, '.f': 5571}


plt.bar(list(dist.keys()), dist.values())
plt.show() 



