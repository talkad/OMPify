import re
import os
import sys
from tqdm import tqdm
from parse_tools import parse
import preprocess as preprocess
import json
import convert_representation as cr
from typing import List


def count_pragma_omp(code):
    counter = 0

    for line in code.split('\n'):
        line_clean = line.strip().lower()

        if line_clean.startswith('#pragma') and ' omp' in line_clean and ' for' in line_clean:
            counter+=1

    return counter


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
    pragma = f'{pragma} '
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


def connect_lines(code):
    '''
    concatenate c lines of code ends with '\'
    '''
    code_buf = []

    for line in code.split('\n'):
        if code_buf and code_buf[-1].rstrip().endswith('\\'):
            code_buf[-1] = code_buf[-1].rstrip()[:-1] + ' ' + line
        else:
            code_buf.append(line)

    code_buf = list(map(lambda line: line.replace('\t', ' ') if line.strip().startswith('#pragma') else line, code_buf))
    return '\n'.join(code_buf)
        


def remove_comp_dir(code):
    code_buf = []

    for line in code.split('\n'):
        if any([line.startswith(comp) for comp in ['#if', '#ifdef', '#ifndef', '#elif', '#else', '#endif']]):
            continue

        code_buf.append(f'{line} ')

    return '\n'.join(code_buf)





class OMP_Extractor():
    def __init__(self, lang='c', replaced=False):
        self.lang = lang
        self.replaced = replaced

        self.samples = []

    def reset(self):
        self.samples = []

    def is_omp_pragma(self, pragma):
        '''
        Returns true if the given pragma is omp
        '''
        line = pragma.decode().lstrip().lower()
        clauses = [clause for clause, _ in parse_openmp_pragma(line)]

        return 'omp' in clauses
        
    def get_pragma(self, pragma):
        '''
        return relevant OpenMP constructs, directives and clauses

        Directive- simd
        Constructs- parallel (for), sections, task, target
        Clause- shared, private, firstprivate, lastprivate, reduction, map
        '''
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'parallel', 'target', 'teams', 'distribute']
        rel_clauses = ['private', 'firstprivate', 'lastprivate', 'reduction', 'simd']
        
        line = pragma.lstrip().lower()
        clauses = parse_openmp_pragma(line)
        clauses_key = [clause for clause, _ in clauses]

        if 'omp' not in clauses_key:                                    # not an OpenMP pragma
            return                  
        
        if not any([const in  rel_constrcuts for const in clauses_key]):  # not contain relevant OpenMP constructs
            return

        rel_pragma = list(filter(lambda clause: clause[0] in rel_constrcuts+rel_clauses, clauses))    # filter out irrelevent clauses

        # unite private/firstprivate/lastprivate as we are not including loop scope
        unified_pragma = []
        private_vars = []
        for clause in rel_pragma:
            if clause[0] in ['private', 'lastprivate', 'firstprivate']:
                private_vars += clause[1].split(',')
            else:
                unified_pragma.append(clause)

        unified_pragma.append(('private', ','.join(private_vars)))
        rel_pragma = unified_pragma

        # rel_pragma = sorted(rel_pragma, key=lambda x: x[0])                                           # sort the pragma
        rel_pragma = list(map(lambda clause: f"{clause[0]}({' '.join(sorted(re.split('[ ,]', clause[1])))})" if clause[1] else clause[0] , rel_pragma))                      # convert to string
        
        return rel_pragma

    def reorder_pragma(self, pragma_lst):
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for']

        pragma_lst = sorted(pragma_lst)
        directives_pragma = list(filter(lambda clause: clause in rel_constrcuts, pragma_lst))
        filtered_pragma = list(filter(lambda clause: clause not in rel_constrcuts, pragma_lst))

        return ' '.join(directives_pragma+filtered_pragma)       

    def detect_omp_fortran(self, node):
        '''      
        The abstract syntax rule of Fortran does not provide a structured block under the OpenMP pragma. 
        However, the code related to the pragma is enclosed by the OpenMP pragma and its corresponding 'end' directive.
        '''
        accumulate_pragmas = []
        is_multiline = False

        for idx, child in enumerate(node.children):
            if  child.type == 'comment' and self.is_omp_pragma(child.text) and b' end' not in child.text:

                pragma = self.get_pragma(child.text.decode())
                if pragma:
                    if is_multiline and accumulate_pragmas:
                        prev_pragma = accumulate_pragmas.pop()
                        accumulate_pragmas.append((prev_pragma[0]+pragma, []))
                    else:
                        accumulate_pragmas.append((pragma, []))
                
                is_multiline = child.text.decode().rstrip().endswith('&')

            elif child.type == 'comment' and self.is_omp_pragma(child.text) and b' end' in child.text and len(accumulate_pragmas) > 0:
                pragma = self.get_pragma(child.text.decode())
                if pragma:
                    pragma_struct = accumulate_pragmas.pop()
                    self.samples.append(('\n'.join(pragma_struct[1]), self.reorder_pragma(pragma_struct[0])))
            else:
                for pragma_struct in accumulate_pragmas:
                    pragma_struct[1].append(child.text.decode())

                self.detect_omp_fortran(child)

    def detect_omp_c(self, node):
        pragma = ''

        for idx, child in enumerate(node.children):
            if child.type == 'preproc_call' and self.is_omp_pragma(child.text):
                pragma = self.get_pragma(child.text)
            elif pragma and child.type in ['compound_statement', 'for_statement']:
                self.samples.append((child.text.decode(), pragma))
                pragma = ''
                self.detect_omp_c(child)
            else:
                pragma = ''
                self.detect_omp_c(child)


    def replace_pragma(self, mappings):
        result = []

        for code, pragma in self.samples:
            # print(code, pragma)

            if isinstance(pragma, List):
                pragma = ' '.join(pragma)

            try:
                clauses = parse_openmp_pragma(pragma)
                replaced_pragma = []

                for clause,vars in clauses:
                    if not vars:
                        replaced_pragma.append((clause,vars))
                    elif clause in ['reduction', 'map']:
                        func = vars[:vars.find(':')]
                        func_vars = vars[vars.find(':')+1:].split()

                        func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
                        replaced_pragma.append((clause,f'{func}:{func_vars}'))
                    else:
                        func_vars = vars.split()

                        func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
                        replaced_pragma.append((clause,func_vars))

                result.append((code, ' '.join(clause[0] if not clause[1] else f'{clause[0]}({clause[1]})' for clause in replaced_pragma)))
            except Exception as e:
                print(e)
                continue

        self.samples = result

    def extract_openmp(self, code):
        '''
        extract for loop and its corresponding openmp pragma (if exists)

        Parameters:
            code: str - textual code representation
        Result:
            A list containing tuples, the first element is the code of the for loop and the second is the pragma (if exists)

        '''
        try:
            if self.lang == 'fortran':
                code = code.lower()

            if self.replaced:
                node = parse(code, lang=self.lang)
                code, mappings = cr.generate_replaced(node)

            node = parse(code, lang=self.lang).root_node

            if self.lang == 'fortran':
                if not code.startswith('subroutine') and not code.startswith('function'):
                    return []
                
                self.detect_omp_fortran(node)
            else:
                self.detect_omp_c(node)
        except RecursionError:
            pass
        
        if self.replaced:
            self.replace_pragma(mappings)

        return self.samples



class DatasetCreatorOMP:
    def __init__(self, data_dir, save_dir, lang='c', replaced=False):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang
        self.replaced = replaced

        self.extractor = OMP_Extractor(lang=self.lang, replaced=self.replaced)

    def is_valid_sample(self, loop, pragma):
        if not loop.startswith('for'): 
            return False
        
        return True

    def fix_pragma(self, pragma):
        if 'private' in pragma:
            idx = pragma.find('private')
            if pragma[idx:].find('(') == -1:
                return pragma[:idx]

        return pragma

    def iterate_corpus(self):
        '''
        Iterate over the HPCorpus and for each function save the following representations:
            1. code
            2. hash
            3. openmp pragma
        '''

        def parse_json(json_file):             
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                data = f.readlines()
                dataset = []

                for line in data:

                    js = json.loads(line.strip())
                    code = js['code'].strip()
                    samples = self.extractor.extract_openmp(code if self.lang=='fortran' else remove_comp_dir(connect_lines(code)))

                    if samples:
                            
                            for loop, pragma in samples:

                                if not self.is_valid_sample(loop, pragma): continue
                                
                                if pragma:
                                    full_pragma = pragma if isinstance(pragma, str) else ' '.join(pragma)

                                    dataset.append({'code': loop,
                                                    'pragma': self.fix_pragma(full_pragma),
                                                    'hash': preprocess.get_hash(loop)
                                    })
                    
                    self.extractor.reset()

                # write the dataset into json
                with open(os.path.join(self.save_dir, json_file), 'w') as data_f:
                    for sample in dataset:
                        data_f.write(json.dumps(sample) + '\n')                

        parse_json(sys.argv[1])


parser = DatasetCreatorOMP('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran/batches', '/home/1010/talkad/Downloads/OMP_Dataset/fortran/replaced', lang='fortran', replaced=True)
parser.iterate_corpus()

