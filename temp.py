import numpy as np 
import matplotlib.pyplot as plt 
  
X = ['Fortran','C']
Ypos = [2753,5176]
Zneg = [3331,6829]
  
X_axis = np.arange(len(X))
  
plt.bar(X_axis - 0.2, Ypos, 0.4, label = 'pos')
plt.bar(X_axis + 0.2, Zneg, 0.4, label = 'neg')
  
plt.xticks(X_axis, X)
plt.title("Examples dist.")
plt.legend()
plt.show()