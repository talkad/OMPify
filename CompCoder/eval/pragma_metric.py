from metrics import *




with open('../result_polycoder_c.log', 'r') as f:
    lines = f.readlines()[3:-1]

    labels, preds = [], []
    for line in lines:
        cols = line.split('|')
        labels.append(cols[1])
        preds.append(cols[2])

    print('private', compare_directive('private', preds, labels))
    print('reduction', compare_directive('reduction', preds, labels))

    print('private var', compare_vars('private', preds, labels))
    print('reduction var', compare_vars('reduction', preds, labels))
    print('reduction operator', compare_vars('reduction', preds, labels, operator=True))


# fortran
# private {'TP': 155, 'FP': 75, 'TN': 415, 'FN': 0, 'Illegal': 127}
# reduction {'TP': 11, 'FP': 2, 'TN': 630, 'FN': 2, 'Illegal': 127}

# private vars - {'TP': 537, 'FP': 24, 'TN': 0, 'FN': 33}
# reduction vars - {'TP': 13, 'FP': 0, 'TN': 0, 'FN': 0}
# reduction vars operator - {'TP': 11, 'FP': 0, 'TN': 0, 'FN': 0}


# fortran tokom
# {'TP': 122, 'FP': 313, 'TN': 285, 'FN': 0, 'Illegal': 215}
# {'TP': 0, 'FP': 0, 'TN': 713, 'FN': 7, 'Illegal': 215}

# {'TP': 4, 'FP': 328, 'TN': 0, 'FN': 325}
# {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}


# c
# {'TP': 753, 'FP': 109, 'TN': 1088, 'FN': 4, 'Illegal': 314}
# {'TP': 61, 'FP': 8, 'TN': 1876, 'FN': 9, 'Illegal': 314}

# {'TP': 2636, 'FP': 69, 'TN': 0, 'FN': 141}
# {'TP': 91, 'FP': 1, 'TN': 0, 'FN': 2}
# {'TP': 60, 'FP': 0, 'TN': 0, 'FN': 0}


# c tokom
# {'TP': 0, 'FP': 0, 'TN': 1644, 'FN': 1, 'Illegal': 1149}
# {'TP': 0, 'FP': 0, 'TN': 1643, 'FN': 2, 'Illegal': 1149}

# {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}




# polycoder:

# private {'TP': 68, 'FP': 37, 'TN': 2666, 'FN': 95, 'Illegal': 7}
# reduction {'TP': 36, 'FP': 36, 'TN': 2760, 'FN': 34, 'Illegal': 7}
# private var {'TP': 108, 'FP': 75, 'TN': 0, 'FN': 75}
# reduction var {'TP': 33, 'FP': 7, 'TN': 0, 'FN': 7}
# reduction operator {'TP': 35, 'FP': 1, 'TN': 0, 'FN': 1}