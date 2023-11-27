from metrics import *
from typing import List
import matplotlib.pyplot as plt
import seaborn as sns
import json


def evaluate_omp(output_file='../result_c_spt_mass.log'):
    with open(output_file, 'r') as f:
        lines = f.readlines()[3:-1]

        labels, preds = [], []
        #################### Vars only ####################
        # for line in lines:
        #     line = line.replace('||', '##')
        #     cols = line.split('|')
        #     if '##' not in cols[2]:
        #         continue
        #     labels.append(cols[1].split('##')[1])
        #     preds.append(cols[2].split('##')[1])

        # print('autoPar', compare_vars_autoPar(preds, labels))

        #################### Full pragma ####################
        
        ## talbe format ##
        # for line in lines:
        #     cols = line.split('|')

        #     labels.append(cols[1])
        #     preds.append(cols[2])

        ## json format ##
        for line in lines:
            sample = json.loads(line.strip())
            labels.append(sample['label'].lower())
            preds.append(sample['prediction'].lower())

        print('private', compare_directive('private', preds, labels))
        print('reduction', compare_directive('reduction', preds, labels))

        print('private var', compare_vars('private', preds, labels))
        print('reduction var', compare_vars('reduction', preds, labels))
        print('reduction operator', compare_vars('reduction', preds, labels, operator=True))


def plot_bar(result: dict, metric='precision', output_file=None):
    labels, values = [], []

    for k, v in result.items():
        labels.append(k)
        values.append(omp_compute_score(v, metric=metric))
    
    plt.figure(figsize=(12,8))
    plt.bar(labels, values)
    plt.ylabel(metric)
    plt.title('OMP Pragma Generation Eval')
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    if not output_file:
        plt.show()
    else:
        plt.savefig(output_file)



def plot_confusion_matrix(ax, cm, title):
    sns.heatmap([[cm['TP'], cm['FP']], [cm['FN'], cm['TN']]], annot=True, fmt='d', cmap='Blues', cbar=False, ax=ax)
    ax.set_xlabel('Predicted Labels')
    ax.set_ylabel('True Labels')
    ax.set_title(title)


def plot_conf_marices(result: dict):
    fig, axs = plt.subplots(2, 3, figsize=(12, 8))

    for (k, cm), ax in zip(result.items(), axs.flatten()):
        plot_confusion_matrix(ax, cm, title=k)

    fig.delaxes(axs[1, 2])  # Remove the empty subplot
    plt.tight_layout()      # Adjust layout

    plt.subplots_adjust(wspace=0.5, hspace=0.5)
    plt.show()









# plot_conf_marices({'private': {'TP': 68, 'FP': 37, 'TN': 2666, 'FN': 95, 'Illegal': 7},
#            'reduction': {'TP': 36, 'FP': 36, 'TN': 2760, 'FN': 34, 'Illegal': 7},
#            'private var': {'TP': 108, 'FP': 75, 'TN': 0, 'FN': 75},
#            'reduction var': {'TP': 33, 'FP': 7, 'TN': 0, 'FN': 7},
#            'reduction operator': {'TP': 35, 'FP': 1, 'TN': 0, 'FN': 1}})












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
# autoPar {'TP': 3185, 'FP': 213, 'TN': 0, 'FN': 657}

#autopar reduction and private:
# private vars  {'TP': 3080, 'FP': 209, 'TN': 0, 'FN': 762}
#reduction vars {'TP': 210, 'FP': 90, 'TN': 0, 'FN': 104}


# GPT3.5 turbo
# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/output.log')
# private {'TP': 86, 'FP': 131, 'TN': 1898, 'FN': 459, 'Illegal': 6}
# reduction {'TP': 122, 'FP': 107, 'TN': 2254, 'FN': 91, 'Illegal': 6}
# private var {'TP': 53, 'FP': 341, 'TN': 0, 'FN': 306}
# reduction var {'TP': 107, 'FP': 38, 'TN': 0, 'FN': 50}
# reduction operator {'TP': 94, 'FP': 0, 'TN': 0, 'FN': 28}

plot_bar({'private': {'TP': 86, 'FP': 131, 'TN': 1898, 'FN': 459, 'Illegal': 6},
           'reduction': {'TP': 122, 'FP': 107, 'TN': 2254, 'FN': 91, 'Illegal': 6},
           'private var': {'TP': 53, 'FP': 341, 'TN': 0, 'FN': 306},
           'reduction var': {'TP': 107, 'FP': 38, 'TN': 0, 'FN': 50},
           'reduction operator': {'TP': 94, 'FP': 0, 'TN': 0, 'FN': 28}}, output_file='GPT_turbo')