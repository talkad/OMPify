import re
import os
from tqdm import tqdm
from parse_tools import parse
import preprocess
import json


def parse_openmp_pragma(pragma):
    '''
    parse OpenMP pragma into meaningful representation

    Parameters:
        pragma: str - string indicating the pragma
    Result:
        A list containing tuples representing the clause and their arguments

    Example:
        input: '#pragma omp for private  (a,b,c) lastprivate(d) schedule(static:8)'
        output: [('pragma', ''), ('omp', ''), ('for', ''), ('private', 'a,b,c'), ('lastprivate', 'd'), ('schedule', 'static:8')]

    '''
    pragma = pragma.decode()
    pattern = r'(\w+(\s*\(.*?\)|\s))'
    matches = re.findall(pattern, pragma)
    clauses = []
    
    for match in matches:
        clause = match[0].strip()

        if '(' in clause:
            clause = clause[:clause.find('(')].strip()
            args = match[1].strip()[1:-1]
            clauses.append((clause, args))
        else:
            clauses.append((clause, ''))
    
    return clauses


class OMP_Extractor():
    def __init__(self, lang='c'):
        self.lang = lang

        self.samples = []

    def reset(self):
        self.sample = []

    def is_omp_for_pragma(self, pragma):
        '''
        Returns true if the given pragma is omp-for
        '''
        line = pragma.lstrip().lower()
        if 'omp ' in line:
            print(line)
        clauses = [clause for clause, _ in parse_openmp_pragma(line)]
        print()
        if self.lang == 'fortran':
            return all([clause in clauses for clause in ['omp', 'do']])
        else:
            return all([clause in clauses for clause in ['omp', 'for']])

    def detect_for(self, node):
        pragma = ''

        for child in node.children:
            if ((not self.lang == 'fortran' and child.type == 'preproc_call') or \
                (self.lang == 'fortran' and child.type == 'comment')) and self.is_omp_for_pragma(child.text):
                pragma = child.text
            elif len(child.type) == 1:
                pass
            elif pragma and child.type in ['compound_statement', 'for_statement', 'do_loop_statement']:
                self.samples.append((child.text.decode(), pragma.decode()))
                pragma = ''
            elif not pragma and child.type in ['for_statement', 'do_loop_statement']:
                self.samples.append((child.text.decode(), ''))
                self.detect_for(child)
            else:
                pragma = ''
                self.detect_for(child)
                

    def extract_for_loops(self, code):
        '''
        extract for loop and its corresponding openmp pragma (if exists)

        Parameters:
            code: str - textual code representation
        Result:
            A list containing tuples, the first element is the code of the for loop and the second is the pragma (if exists)

        '''
        node = parse(code, lang=self.lang).root_node

        self.detect_for(node)
        return self.samples



class DatasetCreatorOMP:
    def __init__(self, data_dir, save_dir, lang='c'):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang

        self.extractor = OMP_Extractor(lang=self.lang)
        
    def iterate_corpus(self):
        '''
        Iterate over the HPCorpus and for each function save the following representations:
            1. code
            2. hash
            3. openmp pragma
        '''

        def parse_json(json_file):             
            dataset = []

            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in tqdm(f):

                    js = json.loads(line.strip())
                    samples = self.extractor.extract_for_loops(js['code'])

                    for loop, target in samples:
                        
                        if target:
                            dataset.append({'code': loop,
                                            'pragma': target,
                                            'hash': preprocess.get_hash(loop)
                            })
                    
                    self.extractor.reset()


                # write the dataset into json
                with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}.jsonl"), 'w') as data_f:
                    for sample in dataset:
                        data_f.write(json.dumps(sample) + '\n')
            
        
        samples = os.listdir(self.data_dir)

        for sample in tqdm(samples):
            print(sample)
            parse_json(sample)


parser = DatasetCreatorOMP('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran', '/home/1010/talkad/Downloads/OMP_Dataset/fortran', lang='fortran')
parser.iterate_corpus()
