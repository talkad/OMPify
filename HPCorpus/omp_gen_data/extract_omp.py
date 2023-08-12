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


class OMP_Extractor():
    def __init__(self, lang='c'):
        self.lang = lang

        self.samples = []

    def reset(self):
        self.sample = []

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
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'task', 'target', 'teams']
        rel_clauses = ['shared', 'private', 'firstprivate', 'lastprivate', 'reduction', 'map', 'simd']
        
        line = pragma.lstrip().lower()
        clauses = parse_openmp_pragma(line)
        clauses_key = [clause for clause, _ in clauses]

        if 'omp' not in clauses_key:                                    # not an OpenMP pragma
            return                  
        
        if not any([const in  rel_constrcuts for const in clauses_key]):  # not contain relevant OpenMP constructs/directives
            print(clauses_key)
            return

        rel_pragma = list(filter(lambda clause: clause[0] in rel_constrcuts+rel_clauses, clauses))    # filter out irrelevent clauses
        rel_pragma = sorted(rel_pragma, key=lambda x: x[0])                                                       # sort the pragma
        rel_pragma = list(map(lambda clause: f"{clause[0]}({''.join(clause[1].split())})" if clause[1] else clause[0] , rel_pragma))                      # convert to string
        
        return ' '.join(rel_pragma)

    def get_omp_construct(self, pragma):
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'task', 'target', 'teams']
        
        line = pragma.decode().lstrip().lower()
        clauses = parse_openmp_pragma(line)
        clauses_key = [clause for clause, _ in clauses]

        return '_'.join(list(filter(lambda clause: clause in rel_constrcuts, clauses_key)))

    def detect_omp_fortran(self, node):
        '''      
        The abstract syntax rule of Fortran does not provide a structured block under the OpenMP pragma. 
        However, the code related to the pragma is enclosed by the OpenMP pragma and its corresponding 'end' directive.
        '''
        accumulate_pragmas = []

        for idx, child in enumerate(node.children):
            if  child.type == 'comment' and self.is_omp_pragma(child.text) and b' end' not in child.text:
                pragma = self.get_pragma(child.text.decode())
                if pragma:
                    accumulate_pragmas.append((pragma, []))
            elif child.type == 'comment' and self.is_omp_pragma(child.text) and b' end' in child.text and len(accumulate_pragmas) > 0:
                pragma = self.get_pragma(child.text.decode())
                if pragma:
                    pragma_struct = accumulate_pragmas.pop()
                    self.samples.append(('\n'.join(pragma_struct[1]), pragma_struct[0]))
            else:
                for pragma_struct in accumulate_pragmas:
                    pragma_struct[1].append(child.text.decode())

                self.detect_omp_fortran(child)

    def detect_omp_c(self, node):
        pragma = ''

        for idx, child in enumerate(node.children):
            if child.type == 'preproc_call' and self.is_omp_pragma(child.text):
                pragma = self.get_pragma(child.text)
                # print(node.children[idx+1].type)
            # elif len(child.type) == 1:
            #     continue
            elif pragma and child.type in ['compound_statement', 'for_statement']:
                self.samples.append((child.text.decode(), pragma))
                pragma = ''
            # elif not pragma and child.type in ['for_statement']:
            #     self.samples.append((child.text.decode(), ''))
            #     self.detect_for(child)
            else:
                pragma = ''
                self.detect_omp_c(child)
                

    def extract_openmp(self, code):
        '''
        extract for loop and its corresponding openmp pragma (if exists)

        Parameters:
            code: str - textual code representation
        Result:
            A list containing tuples, the first element is the code of the for loop and the second is the pragma (if exists)

        '''
        node = parse(code, lang=self.lang).root_node
        try:
            if self.lang == 'fortran':
                self.detect_omp_fortran(node)
            else:
                self.detect_omp_c(node)
        except RecursionError:
            pass
        
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

        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'task', 'target', 'teams']
        rel_clauses = ['shared', 'private', 'firstprivate', 'lastprivate', 'reduction', 'map', 'simd', '_']

        counter = {construct:{clause:0 for clause in rel_clauses} for construct in rel_constrcuts}

        N = 60000
        batch_size = 10000

        def parse_json(json_file):             

            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                data = f.readlines()
                
                for idx, start_index in enumerate(range(0, N, batch_size)):
                    dataset = []
                    end_index = start_index + batch_size
                    print(f'current range {start_index}-{end_index}')
                
                    for line in tqdm(data[start_index:end_index]):

                        js = json.loads(line.strip())
                        samples = self.extractor.extract_openmp(js['code'])

                        for loop, target in samples:
                            
                            if target:
                                ### count ###
                                clauses = parse_openmp_pragma(target)
                                clauses_key = [clause for clause, _ in clauses]

                                for k,v in counter.items():
                                    if k in clauses_key:
                                        counter[k]['_'] += 1

                                    for vv in v:
                                        if k in clauses_key and vv in clauses_key:
                                            counter[k][vv] += 1
                                #############
                                # print(target)
                                # print(loop)
                                dataset.append({'code': loop,
                                                'pragma': target,
                                                'hash': preprocess.get_hash(loop)
                                })
                                # print(counter)
                                # return
                        
                        self.extractor.reset()

                    print(counter)
                    # write the dataset into json
                    with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{idx}.jsonl"), 'w') as data_f:
                        for sample in dataset:
                            data_f.write(json.dumps(sample) + '\n')
                
        
        files = os.listdir(self.data_dir)

        for file in tqdm(files):
            parse_json(file)


# parser = DatasetCreatorOMP('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran', '/home/1010/talkad/Downloads/OMP_Dataset/fortran', lang='fortran')
# parser.iterate_corpus()


code = '''
program OpenMPTasksExample
  use omp_lib

  implicit none

  integer :: num_tasks = 5
  integer :: task_id

  !$omp parallel private(task_id) num_threads(4)
  
  task_id = omp_get_thread_num() + 1

  !$omp task
    call calculate_factorial(task_id)
    print*, 'hey'
  !$omp end task

  print*, 'oooo'

  !$omp end parallel

contains

  subroutine calculate_factorial(task_id)
    integer, intent(in) :: task_id
    integer :: i, n
    integer :: factorial

    n = task_id * 5

    factorial = 1
    do i = 1, n
      factorial = factorial * i
    end do

    print *, "Task", task_id, ": Factorial(", n, ") =", factorial
  end subroutine calculate_factorial

'''
extractor = OMP_Extractor(lang='fortran')
print(extractor.extract_openmp(code))
