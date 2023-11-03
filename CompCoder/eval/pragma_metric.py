from metrics import *




with open('../result_c_spt_mass.log', 'r') as f:
    lines = f.readlines()[3:-1]

    labels, preds = [], []
    for line in lines:
        cols = line.split('|')
        labels.append(cols[1])
        preds.append(cols[2])

    print('autoPar', compare_vars_autoPar(preds, labels))

    # print('private', compare_directive('private', preds, labels))
    # print('reduction', compare_directive('reduction', preds, labels))

    # print('private var', compare_vars('private', preds, labels))
    # print('reduction var', compare_vars('reduction', preds, labels))
    # print('reduction operator', compare_vars('reduction', preds, labels, operator=True))


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
# private {'TP': 753, 'FP': 109, 'TN': 1088, 'FN': 4, 'Illegal': 314}
# reduction {'TP': 61, 'FP': 8, 'TN': 1876, 'FN': 9, 'Illegal': 314}

# private var {'TP': 2636, 'FP': 69, 'TN': 0, 'FN': 141}
# reduction var {'TP': 91, 'FP': 1, 'TN': 0, 'FN': 2}
# reduction operator {'TP': 60, 'FP': 0, 'TN': 0, 'FN': 0}



# c tokom
# private {'TP': 0, 'FP': 0, 'TN': 1644, 'FN': 1, 'Illegal': 1149}
# reduction {'TP': 0, 'FP': 0, 'TN': 1643, 'FN': 2, 'Illegal': 1149}

# private var {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# reduction var {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# reduction operator {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}



# spt c:
# private {'TP': 626, 'FP': 334, 'TN': 847, 'FN': 0, 'Illegal': 461}
# reduction {'TP': 0, 'FP': 0, 'TN': 1774, 'FN': 33, 'Illegal': 461}

# private var {'TP': 1731, 'FP': 87, 'TN': 0, 'FN': 167}
# reduction var {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}
# reduction operator {'TP': 0, 'FP': 0, 'TN': 0, 'FN': 0}



# polycoder c:

# private {'TP': 68, 'FP': 37, 'TN': 2666, 'FN': 95, 'Illegal': 7}
# reduction {'TP': 36, 'FP': 36, 'TN': 2760, 'FN': 34, 'Illegal': 7}
# private var {'TP': 108, 'FP': 75, 'TN': 0, 'FN': 75}
# reduction var {'TP': 33, 'FP': 7, 'TN': 0, 'FN': 7}
# reduction operator {'TP': 35, 'FP': 1, 'TN': 0, 'FN': 1}


# AutoParLLM
# private vars {'TP': 5854, 'FP': 778, 'TN': 0, 'FN': 1971}
