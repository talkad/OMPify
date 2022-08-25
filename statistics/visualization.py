import numpy as np 
import matplotlib.pyplot as plt 
  
X = ['Fortran','C']
Ypos = [5075,11772]
Zneg = [1099,10042]
  
X_axis = np.arange(len(X))
  
plt.bar(X_axis - 0.2, Ypos, 0.4, label = 'success')
plt.bar(X_axis + 0.2, Zneg, 0.4, label = 'fail')
  
plt.xticks(X_axis, X)
plt.title("Files Parsing")
plt.legend()
plt.show()