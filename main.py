import matplotlib.pyplot as plt
from optparse import OptionParser
from git_clone import loader, extractor
from visualization import pragmasCounter
from parsers import cParser, cppParser, fortranParser
from parsers.fake_headers import fake
import numpy as np
from sklearn.linear_model import Ridge


def load_repos():
    loader.load()
    extractor.scan_dir(fake.REPOS_DIR)

def split(a, n):
    k, m = divmod(len(a), n)
    return (a[i*k+min(i, m):(i+1)*k+min(i+1, m)] for i in range(n))

def show_stats():
    omp_usage = loader.load(is_dry=True)
    pragmas, _, files = pragmasCounter.scan_dir(fake.REPOS_OMP_DIR)

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

def parse(prog_lang):
    assert prog_lang.startswith('(') and prog_lang.endswith(')')

    for lang in prog_lang[1:][:-1].lower().split('|'):
        if lang == 'c':
            parser = cParser.CLoopParser('repositories_openMP', 'c_loops')
        elif lang == 'cpp':
            parser = cppParser.CppLoopParser('repositories_openMP', 'cpp_loops')
        elif lang == 'fortran':
            parser = fortranParser.FortranLoopParser('repositories_openMP', 'fortran_loops')
        else: 
            continue

        parser.scan_dir()



def main():
    parser = OptionParser(usage="usage: python %prog [options]",
                          version="%prog 1.0")
    parser.add_option("-l", "--load",
                      action="store_true",
                      default=False,
                      help="load omp repositories from github")
    parser.add_option("-s", "--stats",
                      action="store_true",
                      default=False,
                      help="show statistical usage of openMP")                     
    parser.add_option("-p", "--parse",
                      dest="prog_lang",
                      help="parse the given programming languages. example: -p \"(c|cpp)\"")
    (options, args) = parser.parse_args()

    if options.load:
        load()
    
    if options.stats:
        show_stats()
        
    if options.prog_lang is not None:
        parse(options.prog_lang)

if __name__ == '__main__':
    main()