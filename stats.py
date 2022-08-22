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


# pragmas counter output
# ({'nowait': 7652, 'private': 60042, 'firstprivate': 15672, 'lastprivate': 16580, 'shared': 10960, 'reduction': 36193, 'static_schedule': 18481, 'dynamic_schedule': 13102}, 
# {'.cpp': 169966, '.c': 70113, '.f90': 15285, '.h': 3246, '.cc': 1193, '.f': 5571, '.f95': 337, '.f03': 36, '.cxx': 132}, 
# {'.cpp': 14421, '.c': 20174, '.f90': 4367, '.h': 1065, '.cc': 575, '.f': 1528, '.f95': 279, '.f03': 10, '.cxx': 71})

dist = {'.cpp': 169966, '.c': 70113, '.f90': 15285, '.h': 3246, '.cc': 1193, '.f': 5571, '.f95': 337, '.f03': 36, '.cxx': 132}


plt.bar(list(dist.keys()), dist.values())
plt.show() 



