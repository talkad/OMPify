import numpy as np 
import matplotlib.pyplot as plt 
from sklearn.linear_model import Ridge
from visualization import pragmasCounter

  
def split(a, n):
    k, m = divmod(len(a), n)
    return (a[i*k+min(i, m):(i+1)*k+min(i+1, m)] for i in range(n))

def show_stats(omp_dir):
    omp_usage = loader.load(is_dry=True)
    pragmas, _, files = pragmasCounter.scan_dir(omp_dir)

    labels = []
    cur = ''

    for label in list(omp_usage.keys()):
        year = label.split('-')[1]

        if cur != year:
            labels.append(year)
            cur = year
        else:
            labels.append('')

    ax1 = plt.subplot2grid((2, 5), (0, 0), colspan=5)
    ax1.bar(list(omp_usage.keys()), omp_usage.values())
    ax1.set_xticklabels(labels)

    n = 2
    splitted_list = list(split(list(omp_usage.values()), n))
    start = 0
    for idx in range(n):
        lr = Ridge()
        l = splitted_list[idx]
        X = np.array(list(range(start, start + len(l))))
        lr.fit(X.reshape(-1, 1), np.array(l))

        ax1.plot(X, lr.coef_*X+lr.intercept_, color='orange')
        start += len(l)

    ax2 = plt.subplot2grid((2, 5), (1, 0), colspan=2)
    ax2.bar(list(files.keys()), files.values())

    ax3 = plt.subplot2grid((2, 5), (1, 2), colspan=3)
    ax3.bar(list(pragmas.keys()), pragmas.values())
    plt.xticks(rotation='-15')

    plt.show()    
