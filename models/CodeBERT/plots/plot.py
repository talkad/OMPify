import matplotlib.pyplot as plt 

plt.plot([1,2,3,4,5], [79.44, 84.14, 85.14, 86.11,  86.64], label='codeBERT - C')
plt.plot([1,2,3,4,5], [76.23, 71.99, 76.76, 75.11, 75.54], label='codeBERT - replaced')
plt.plot([1,2,3,4,5], [77.07, 82.18, 84.05, 83.68, 85.17], label='codeBERT - curriculum')

plt.plot([1,2,3,4,5], [79.00, 81.83, 85.04, 85.5, 86.13], label='SPT - C')
plt.plot([1,2,3,4,5], [79.22, 80.09, 84.01, 84.2, 85.48], label='SPT - C + AST')
plt.plot([1,2,3,4,5], [80.02, 82.11, 83.95, 85.12, 85.33], label='SPT - C + AST + curriculum')

plt.title('Accuracy over epochs')
plt.xlabel('epoch')
plt.ylabel('accuracy')

plt.legend()
plt.show()