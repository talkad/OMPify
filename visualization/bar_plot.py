import numpy as np 
import matplotlib.pyplot as plt 
  
X = ['Fortran','C', 'C++']
Ypos = [5075,16942, 14421]
Zneg = [1099,3861, 248]

# Ypos = [2753,13238, 6952]
# Zneg = [3331,36593, 24098]
  
X_axis = np.arange(len(X))
  
plt.bar(X_axis - 0.2, Ypos, 0.4, label = 'pos', color='#66CC00')
plt.bar(X_axis + 0.2, Zneg, 0.4, label = 'neg', color='#DD0000')
  
plt.xticks(X_axis, X)
plt.title("Files Parsing")
plt.legend()
plt.show()