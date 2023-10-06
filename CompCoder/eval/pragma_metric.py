from eval.metrics import *




# with open('result_fortran_tokom.log', 'r') as f:
#     lines = f.readlines()[3:-1]

#     labels, preds = [], []
#     for line in lines:
#         cols = line.split('|')
#         labels.append(cols[1])
#         preds.append(cols[2])

#     print(compare_directive('private', preds, labels))


# fortran
# private {'TP': 155, 'FP': 75, 'TN': 415, 'FN': 0, 'Illegal': 127}
# reduction {'TP': 11, 'FP': 2, 'TN': 630, 'FN': 2, 'Illegal': 127}
# {'TP': 0, 'FP': 0, 'TN': 645, 'FN': 0, 'Illegal': 127}

# private vars - {'TP': 537, 'FP': 24, 'TN': 0, 'FN': 33}
# reduction vars - {'TP': 13, 'FP': 0, 'TN': 0, 'FN': 0}
# reduction vars operator - {'TP': 11, 'FP': 0, 'TN': 0, 'FN': 0}


