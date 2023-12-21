from metrics import *
from typing import List
import matplotlib.pyplot as plt
import seaborn as sns
import json


def evaluate_omp(output_file='../result_c_spt_mass.log'):
    with open(output_file, 'r') as f:
        lines = f.readlines()[3:-1]

        labels, preds = [], []
        
        # ## talbe format ##
        # for line in lines:
        #     cols = line.split('|')

        #     labels.append(cols[1])
        #     preds.append(cols[2])

        ## json format ##
        for line in lines:
            sample = json.loads(line.strip())
            labels.append(sample['label'].lower())
            preds.append(sample['pred'].lower())

        # res = {}
        # for label in labels:
        #     d = pragma2dict(label)
        #     if 'reduction' in d:
        #         vs = d['reduction']['vars']
        #         amount = len(vs)
        #         res[amount] = 1 if amount not in res else res[amount]+1

        # print(dict(sorted(res.items())))

        for max_vars in [1,2,3,4,5,100]:
            print(f'# {max_vars}')

            for clause in ['private', 'reduction']:
                preds2, labels2 = [], []

                for label, pred in zip(labels, preds):
                    d = pragma2dict(label)
                    if clause in d:
                        vs = d[clause]['vars']
                        amount = len(vs)
                        
                        if amount <= max_vars:
                            labels2.append(label)
                            preds2.append(pred)

                private_conf = compare_vars(clause, preds2, labels2)
                print(f'{clause} var', private_conf, omp_compute_score(private_conf))


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


# # evaluate_omp('/mnt/lbosm1/home/Share/code-lms/polycoder/tasks/omp/hf/final_results/poly_tokom.log')

# # private amount vars dist:
# # {1: 239, 2: 115, 3: 53, 4: 43, 5: 21, 6: 18, 7: 15, 8: 14, 9: 1, 10: 6, 11: 2, 12: 2, 13: 3, 14: 5, 15: 2, 19: 1, 20: 1, 22: 2, 26: 2, 35: 1, 38: 1}
# vr_num = [239, 354, 407, 450, 471,  547]
# gpt = [0.091, 0.112, 0.117, 0.1213, 0.116, 0.075]
# comp_bpe = [0.847, 0.734, 0.708, 0.677, 0.659, 0.553]
# comp_replaced_bpe = [0.767, 0.704, 0.689, 0.627, 0.611, 0.484]
# # comp_tokom = [0.191, 0.197, 0.178, 0.170, 0.163, 0.131]
# gpt_replaced = [0.014, 0.047, 0.054, 0.06, 0.068, 0.064]


# # reduction amount vars dist:
# # {1: 173, 2: 26, 3: 7, 4: 4, 5: 1, 6: 2, 7: 1}
# vr_num = [173, 199, 206, 210, 211, 214]
# gpt = [0.875, 0.711, 0.6325, 0.594, 0.571, 0.551]
# comp_bpe = [0.670, 0.646, 0.629, 0.636, 0.631, 0.638]
# comp_replaced_bpe = [0.728, 0.707, 0.688, 0.684, 0.680, 0.649]
# # comp_tokom = [0.042, 0.045, 0.043, 0.041, 0.041, 0.035]
# gpt_replaced = [0.865, 0.658, 0.613, 0.589, 0.589, 0.5]

# bars = [('Compcoder', comp_bpe), ('Compcoder Replaced', comp_replaced_bpe), 
#         ('GPT', gpt), ('GPT Replaced', comp_tokom)]

# import numpy as np

# fig, axs = plt.subplots(nrows=1, ncols=2, figsize=(12, 5))
# # Plot the first subplot
# x1 = np.arange(6)
# xs = ['<=1', '<=2', '<=3', '<=4', '<=5', 'all']

# axs[0].bar(xs, vr_num)
# axs[0].set_title('samples dist amount of vars')
# axs[0].set_xlabel('vars per clause')
# axs[0].set_ylabel('# samples')

# # Plot the second subplot with multiple bars
# width = 0.2  # Width of each bar
# x2 = np.arange(6)  # X-axis positions for each set of bars

# for i, (label, row) in enumerate(bars):
#     axs[1].bar(x2 + i * width, row, width=width, label=label)

# axs[1].set_title('variable accuracy')
# axs[1].set_xlabel('vars per clause')
# axs[1].set_ylabel('Acc')
# axs[1].set_xticks(x2 + (len(bars) - 1) * width / 2)
# axs[1].set_xticklabels(xs)
# axs[1].legend()

# axs[0].grid()
# axs[1].grid()

# # Adjust layout for better visibility
# plt.tight_layout()

# # Show the plot
# plt.show()





# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed.log')
#GPT3.5
# # 1
# private var {'TP': 8, 'FP': 63, 'TN': 0, 'FN': 16} 0.09195402298850575
# reduction var {'TP': 98, 'FP': 8, 'TN': 0, 'FN': 6} 0.875
# # 2
# private var {'TP': 16, 'FP': 92, 'TN': 0, 'FN': 34} 0.11267605633802817
# reduction var {'TP': 101, 'FP': 18, 'TN': 0, 'FN': 23} 0.7112676056338029
# # 3
# private var {'TP': 23, 'FP': 115, 'TN': 0, 'FN': 57} 0.11794871794871795
# reduction var {'TP': 105, 'FP': 27, 'TN': 0, 'FN': 34} 0.6325301204819277
# # 4
# private var {'TP': 29, 'FP': 131, 'TN': 0, 'FN': 79} 0.12133891213389121
# reduction var {'TP': 107, 'FP': 33, 'TN': 0, 'FN': 40} 0.5944444444444444
# # 5
# private var {'TP': 35, 'FP': 159, 'TN': 0, 'FN': 107} 0.11627906976744186
# reduction var {'TP': 108, 'FP': 37, 'TN': 0, 'FN': 44} 0.5714285714285714
# # inf
# private var {'TP': 53, 'FP': 341, 'TN': 0, 'FN': 306} 0.07571428571428572
# reduction var {'TP': 108, 'FP': 38, 'TN': 0, 'FN': 50} 0.5510204081632653


# GPT replaced
# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed_replaced.log')
# # 1
# private var {'TP': 2, 'FP': 114, 'TN': 0, 'FN': 20} 0.014705882352941176
# reduction var {'TP': 77, 'FP': 6, 'TN': 0, 'FN': 6} 0.8651685393258427
# # 2
# private var {'TP': 8, 'FP': 129, 'TN': 0, 'FN': 32} 0.047337278106508875
# reduction var {'TP': 81, 'FP': 16, 'TN': 0, 'FN': 26} 0.6585365853658537
# # 3
# private var {'TP': 12, 'FP': 162, 'TN': 0, 'FN': 46} 0.05454545454545454
# reduction var {'TP': 81, 'FP': 19, 'TN': 0, 'FN': 32} 0.6136363636363636
# # 4
# private var {'TP': 14, 'FP': 166, 'TN': 0, 'FN': 52} 0.0603448275862069
# reduction var {'TP': 82, 'FP': 22, 'TN': 0, 'FN': 35} 0.5899280575539568
# # 5
# private var {'TP': 17, 'FP': 170, 'TN': 0, 'FN': 63} 0.068
# reduction var {'TP': 82, 'FP': 22, 'TN': 0, 'FN': 35} 0.5899280575539568
# # 100
# private var {'TP': 28, 'FP': 255, 'TN': 0, 'FN': 154} 0.06407322654462243
# reduction var {'TP': 84, 'FP': 33, 'TN': 0, 'FN': 51} 0.5



# compcoder BPE
# # 1
# private var {'TP': 172, 'FP': 12, 'TN': 0, 'FN': 19} 0.8472906403940886
# reduction var {'TP': 114, 'FP': 37, 'TN': 0, 'FN': 19} 0.6705882352941176
# # 2
# private var {'TP': 302, 'FP': 42, 'TN': 0, 'FN': 67} 0.7347931873479319
# reduction var {'TP': 139, 'FP': 44, 'TN': 0, 'FN': 32} 0.6465116279069767
# # 3
# private var {'TP': 411, 'FP': 61, 'TN': 0, 'FN': 108} 0.7086206896551724
# reduction var {'TP': 163, 'FP': 52, 'TN': 0, 'FN': 44} 0.6293436293436293
# # 4
# private var {'TP': 514, 'FP': 88, 'TN': 0, 'FN': 157} 0.6772068511198946
# reduction var {'TP': 175, 'FP': 52, 'TN': 0, 'FN': 48} 0.6363636363636364
# # 5
# private var {'TP': 580, 'FP': 111, 'TN': 0, 'FN': 189} 0.6590909090909091
# reduction var {'TP': 183, 'FP': 54, 'TN': 0, 'FN': 53} 0.6310344827586207
# # inf
# private var {'TP': 838, 'FP': 251, 'TN': 0, 'FN': 424} 0.5538664904163912
# reduction var {'TP': 189, 'FP': 54, 'TN': 0, 'FN': 53} 0.6385135135135135



# compcoder replaced BPE
# # 1
# private var {'TP': 241, 'FP': 44, 'TN': 0, 'FN': 29} 0.767515923566879
# reduction var {'TP': 134, 'FP': 33, 'TN': 0, 'FN': 17} 0.7282608695652174
# # 2
# private var {'TP': 401, 'FP': 81, 'TN': 0, 'FN': 87} 0.7047451669595782
# reduction var {'TP': 169, 'FP': 40, 'TN': 0, 'FN': 30} 0.7071129707112971
# # 3
# private var {'TP': 534, 'FP': 110, 'TN': 0, 'FN': 131} 0.6890322580645162
# reduction var {'TP': 172, 'FP': 42, 'TN': 0, 'FN': 36} 0.688
# # 4
# private var {'TP': 620, 'FP': 155, 'TN': 0, 'FN': 213} 0.6275303643724697
# reduction var {'TP': 180, 'FP': 43, 'TN': 0, 'FN': 40} 0.6844106463878327
# # 5
# private var {'TP': 756, 'FP': 194, 'TN': 0, 'FN': 286} 0.6116504854368932
# reduction var {'TP': 183, 'FP': 44, 'TN': 0, 'FN': 42} 0.6802973977695167
# # inf
# private var {'TP': 1025, 'FP': 382, 'TN': 0, 'FN': 707} 0.4848628192999054
# reduction var {'TP': 202, 'FP': 50, 'TN': 0, 'FN': 59} 0.6495176848874598


# compcoder tokom
# # 1
# private var {'TP': 51, 'FP': 137, 'TN': 0, 'FN': 78} 0.19172932330827067
# reduction var {'TP': 4, 'FP': 48, 'TN': 0, 'FN': 43} 0.042105263157894736
# # 2
# private var {'TP': 101, 'FP': 226, 'TN': 0, 'FN': 184} 0.19765166340508805
# reduction var {'TP': 5, 'FP': 56, 'TN': 0, 'FN': 50} 0.04504504504504504
# # 3
# private var {'TP': 140, 'FP': 332, 'TN': 0, 'FN': 311} 0.17879948914431673
# reduction var {'TP': 5, 'FP': 57, 'TN': 0, 'FN': 53} 0.043478260869565216
# # 4
# private var {'TP': 160, 'FP': 384, 'TN': 0, 'FN': 395} 0.1703940362087327
# reduction var {'TP': 5, 'FP': 58, 'TN': 0, 'FN': 57} 0.041666666666666664
# # 5
# private var {'TP': 192, 'FP': 466, 'TN': 0, 'FN': 514} 0.16382252559726962
# reduction var {'TP': 5, 'FP': 58, 'TN': 0, 'FN': 57} 0.041666666666666664
# # 100
# private var {'TP': 282, 'FP': 835, 'TN': 0, 'FN': 1020} 0.13196069255966308
# reduction var {'TP': 5, 'FP': 65, 'TN': 0, 'FN': 69} 0.03597122302158273

































# new vars if the clause exists only in label and not both as in the previous results.



# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed_source.log')
#GPT3.5
# # 1
# private var {'TP': 8, 'FP': 63, 'TN': 0, 'FN': 231} 0.026490066225165563
# reduction var {'TP': 98, 'FP': 8, 'TN': 0, 'FN': 75} 0.5414364640883977
# # 2
# private var {'TP': 16, 'FP': 92, 'TN': 0, 'FN': 450} 0.02867383512544803
# reduction var {'TP': 101, 'FP': 18, 'TN': 0, 'FN': 124} 0.4156378600823045
# # 3
# private var {'TP': 23, 'FP': 115, 'TN': 0, 'FN': 602} 0.031081081081081083
# reduction var {'TP': 105, 'FP': 27, 'TN': 0, 'FN': 141} 0.38461538461538464
# # 4
# private var {'TP': 29, 'FP': 131, 'TN': 0, 'FN': 767} 0.03128371089536138
# reduction var {'TP': 107, 'FP': 33, 'TN': 0, 'FN': 155} 0.36271186440677966
# # 5
# private var {'TP': 35, 'FP': 159, 'TN': 0, 'FN': 864} 0.0330812854442344
# reduction var {'TP': 108, 'FP': 37, 'TN': 0, 'FN': 159} 0.35526315789473684
# # 100
# private var {'TP': 53, 'FP': 341, 'TN': 0, 'FN': 1622} 0.02628968253968254
# reduction var {'TP': 108, 'FP': 38, 'TN': 0, 'FN': 178} 0.3333333333333333


# GPT replaced
# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed_replaced.log')
# # 1
# private var {'TP': 2, 'FP': 114, 'TN': 0, 'FN': 364} 0.004166666666666667
# reduction var {'TP': 77, 'FP': 6, 'TN': 0, 'FN': 124} 0.3719806763285024
# # 2
# private var {'TP': 8, 'FP': 129, 'TN': 0, 'FN': 626} 0.010484927916120577
# reduction var {'TP': 81, 'FP': 16, 'TN': 0, 'FN': 168} 0.30566037735849055
# # 3
# private var {'TP': 12, 'FP': 162, 'TN': 0, 'FN': 847} 0.011753183153770812
# reduction var {'TP': 81, 'FP': 19, 'TN': 0, 'FN': 180} 0.2892857142857143
# # 4
# private var {'TP': 14, 'FP': 166, 'TN': 0, 'FN': 1021} 0.011656952539550375
# reduction var {'TP': 82, 'FP': 22, 'TN': 0, 'FN': 195} 0.27424749163879597
# # 5
# private var {'TP': 17, 'FP': 170, 'TN': 0, 'FN': 1252} 0.0118137595552467
# reduction var {'TP': 82, 'FP': 22, 'TN': 0, 'FN': 200} 0.26973684210526316
# # 100
# private var {'TP': 28, 'FP': 255, 'TN': 0, 'FN': 2261} 0.0110062893081761
# reduction var {'TP': 84, 'FP': 33, 'TN': 0, 'FN': 234} 0.23931623931623933



# compcoder BPE
# evaluate_omp('/mnt/lbosm1/home/Share/code-lms/polycoder/tasks/omp/hf/final_results/poly_bpe.log')
# # 1
# private var {'TP': 172, 'FP': 12, 'TN': 0, 'FN': 65} 0.6907630522088354
# reduction var {'TP': 114, 'FP': 37, 'TN': 0, 'FN': 45} 0.5816326530612245
# # 2
# private var {'TP': 302, 'FP': 42, 'TN': 0, 'FN': 152} 0.6088709677419355
# reduction var {'TP': 139, 'FP': 44, 'TN': 0, 'FN': 76} 0.5366795366795367
# # 3
# private var {'TP': 411, 'FP': 61, 'TN': 0, 'FN': 214} 0.5991253644314869
# reduction var {'TP': 163, 'FP': 52, 'TN': 0, 'FN': 100} 0.5174603174603175
# # 4
# private var {'TP': 514, 'FP': 88, 'TN': 0, 'FN': 279} 0.583427922814983
# reduction var {'TP': 175, 'FP': 52, 'TN': 0, 'FN': 104} 0.5287009063444109
# # 5
# private var {'TP': 580, 'FP': 111, 'TN': 0, 'FN': 321} 0.5731225296442688
# reduction var {'TP': 183, 'FP': 54, 'TN': 0, 'FN': 109} 0.5289017341040463
# # 100
# private var {'TP': 838, 'FP': 251, 'TN': 0, 'FN': 637} 0.4855156431054461
# reduction var {'TP': 189, 'FP': 54, 'TN': 0, 'FN': 122} 0.5178082191780822


# compcoder replaced BPE
# evaluate_omp('/mnt/lbosm1/home/Share/code-lms/polycoder/tasks/omp/hf/final_results/poly_replaced_bpe.log')
# # 1
# private var {'TP': 241, 'FP': 44, 'TN': 0, 'FN': 114} 0.6040100250626567
# reduction var {'TP': 134, 'FP': 33, 'TN': 0, 'FN': 59} 0.5929203539823009
# # 2
# private var {'TP': 401, 'FP': 81, 'TN': 0, 'FN': 220} 0.5712250712250713
# reduction var {'TP': 169, 'FP': 40, 'TN': 0, 'FN': 75} 0.5950704225352113
# # 3
# private var {'TP': 534, 'FP': 110, 'TN': 0, 'FN': 309} 0.5603357817418678
# reduction var {'TP': 172, 'FP': 42, 'TN': 0, 'FN': 81} 0.5830508474576271
# # 4
# private var {'TP': 620, 'FP': 155, 'TN': 0, 'FN': 399} 0.5281090289608177
# reduction var {'TP': 180, 'FP': 43, 'TN': 0, 'FN': 89} 0.5769230769230769
# # 5
# private var {'TP': 756, 'FP': 194, 'TN': 0, 'FN': 487} 0.5260960334029228
# reduction var {'TP': 183, 'FP': 44, 'TN': 0, 'FN': 91} 0.5754716981132075
# # 100
# private var {'TP': 1025, 'FP': 382, 'TN': 0, 'FN': 945} 0.43579931972789115
# reduction var {'TP': 202, 'FP': 50, 'TN': 0, 'FN': 108} 0.5611111111111111




































































# new vars if the clause exists only in label and not both as in the previous results.



# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed_source.log')
#GPT3.5
# # 1
# private var {'TP': 18, 'FP': 98, 'TN': 0, 'FN': 221} 0.05341246290801187
# reduction var {'TP': 100, 'FP': 8, 'TN': 0, 'FN': 73} 0.5524861878453039
# # 2
# private var {'TP': 37, 'FP': 116, 'TN': 0, 'FN': 429} 0.06357388316151202
# reduction var {'TP': 119, 'FP': 9, 'TN': 0, 'FN': 106} 0.5085470085470085
# # 3
# private var {'TP': 58, 'FP': 125, 'TN': 0, 'FN': 567} 0.07733333333333334
# reduction var {'TP': 134, 'FP': 9, 'TN': 0, 'FN': 112} 0.5254901960784314
# # 4
# private var {'TP': 78, 'FP': 127, 'TN': 0, 'FN': 718} 0.08450704225352113
# reduction var {'TP': 142, 'FP': 9, 'TN': 0, 'FN': 120} 0.5239852398523985
# # 5
# private var {'TP': 102, 'FP': 137, 'TN': 0, 'FN': 797} 0.09845559845559845
# reduction var {'TP': 147, 'FP': 9, 'TN': 0, 'FN': 120} 0.532608695652174
# # 100
# private var {'TP': 267, 'FP': 180, 'TN': 0, 'FN': 1408} 0.14393530997304582
# reduction var {'TP': 153, 'FP': 9, 'TN': 0, 'FN': 133} 0.5186440677966102


# GPT replaced
# evaluate_omp('/mnt/lbosm1/home/Share/OMPify/CompCoder/comparison_models/GPT3.5_Turbo/processed_replaced.log')
# # 1
# private var {'TP': 9, 'FP': 123, 'TN': 0, 'FN': 357} 0.018404907975460124
# reduction var {'TP': 80, 'FP': 10, 'TN': 0, 'FN': 121} 0.3791469194312796
# # 2
# private var {'TP': 17, 'FP': 136, 'TN': 0, 'FN': 617} 0.02207792207792208
# reduction var {'TP': 102, 'FP': 10, 'TN': 0, 'FN': 147} 0.3938223938223938
# # 3
# private var {'TP': 33, 'FP': 157, 'TN': 0, 'FN': 826} 0.03248031496062992
# reduction var {'TP': 105, 'FP': 12, 'TN': 0, 'FN': 156} 0.38461538461538464
# # 4
# private var {'TP': 39, 'FP': 157, 'TN': 0, 'FN': 996} 0.03271812080536913
# reduction var {'TP': 109, 'FP': 12, 'TN': 0, 'FN': 168} 0.3771626297577855
# # 5
# private var {'TP': 45, 'FP': 158, 'TN': 0, 'FN': 1224} 0.03153468815697267
# reduction var {'TP': 109, 'FP': 12, 'TN': 0, 'FN': 173} 0.3707482993197279
# # 100
# private var {'TP': 111, 'FP': 188, 'TN': 0, 'FN': 2178} 0.04481227291077917
# reduction var {'TP': 127, 'FP': 12, 'TN': 0, 'FN': 191} 0.38484848484848483



# compcoder BPE
# evaluate_omp('/mnt/lbosm1/home/Share/code-lms/polycoder/tasks/omp/hf/final_results/poly_bpe.log')
# # 1
# private var {'TP': 172, 'FP': 12, 'TN': 0, 'FN': 65} 0.6907630522088354
# reduction var {'TP': 114, 'FP': 37, 'TN': 0, 'FN': 45} 0.5816326530612245
# # 2
# private var {'TP': 302, 'FP': 42, 'TN': 0, 'FN': 152} 0.6088709677419355
# reduction var {'TP': 139, 'FP': 44, 'TN': 0, 'FN': 76} 0.5366795366795367
# # 3
# private var {'TP': 411, 'FP': 61, 'TN': 0, 'FN': 214} 0.5991253644314869
# reduction var {'TP': 163, 'FP': 52, 'TN': 0, 'FN': 100} 0.5174603174603175
# # 4
# private var {'TP': 514, 'FP': 88, 'TN': 0, 'FN': 279} 0.583427922814983
# reduction var {'TP': 175, 'FP': 52, 'TN': 0, 'FN': 104} 0.5287009063444109
# # 5
# private var {'TP': 580, 'FP': 111, 'TN': 0, 'FN': 321} 0.5731225296442688
# reduction var {'TP': 183, 'FP': 54, 'TN': 0, 'FN': 109} 0.5289017341040463
# # 100
# private var {'TP': 838, 'FP': 251, 'TN': 0, 'FN': 637} 0.4855156431054461
# reduction var {'TP': 189, 'FP': 54, 'TN': 0, 'FN': 122} 0.5178082191780822


# compcoder replaced BPE
# evaluate_omp('/mnt/lbosm1/home/Share/code-lms/polycoder/tasks/omp/hf/final_results/poly_replaced_bpe.log')
# # 1
# private var {'TP': 241, 'FP': 44, 'TN': 0, 'FN': 114} 0.6040100250626567
# reduction var {'TP': 134, 'FP': 33, 'TN': 0, 'FN': 59} 0.5929203539823009
# # 2
# private var {'TP': 401, 'FP': 81, 'TN': 0, 'FN': 220} 0.5712250712250713
# reduction var {'TP': 169, 'FP': 40, 'TN': 0, 'FN': 75} 0.5950704225352113
# # 3
# private var {'TP': 534, 'FP': 110, 'TN': 0, 'FN': 309} 0.5603357817418678
# reduction var {'TP': 172, 'FP': 42, 'TN': 0, 'FN': 81} 0.5830508474576271
# # 4
# private var {'TP': 620, 'FP': 155, 'TN': 0, 'FN': 399} 0.5281090289608177
# reduction var {'TP': 180, 'FP': 43, 'TN': 0, 'FN': 89} 0.5769230769230769
# # 5
# private var {'TP': 756, 'FP': 194, 'TN': 0, 'FN': 487} 0.5260960334029228
# reduction var {'TP': 183, 'FP': 44, 'TN': 0, 'FN': 91} 0.5754716981132075
# # 100
# private var {'TP': 1025, 'FP': 382, 'TN': 0, 'FN': 945} 0.43579931972789115
# reduction var {'TP': 202, 'FP': 50, 'TN': 0, 'FN': 108} 0.5611111111111111