import re
import os
import sys
from tqdm import tqdm
from parse_tools import parse
import preprocess
import json
import convert_representation as cr


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
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'target', 'task', 'teams']
        rel_clauses = ['shared', 'private', 'firstprivate', 'lastprivate', 'reduction', 'map', 'simd']
        
        line = pragma.lstrip().lower()
        clauses = parse_openmp_pragma(line)
        clauses_key = [clause for clause, _ in clauses]

        if 'omp' not in clauses_key:                                    # not an OpenMP pragma
            return                  
        
        if not any([const in  rel_constrcuts+rel_clauses for const in clauses_key]):  # not contain relevant OpenMP constructs/directives
            return

        rel_pragma = list(filter(lambda clause: clause[0] in rel_constrcuts+rel_clauses, clauses))    # filter out irrelevent clauses
        rel_pragma = sorted(rel_pragma, key=lambda x: x[0])                                                       # sort the pragma
        rel_pragma = list(map(lambda clause: f"{clause[0]}({' '.join(sorted(re.split('[ ,]', clause[1])))})" if clause[1] else clause[0] , rel_pragma))                      # convert to string
        
        return rel_pragma

    def get_omp_construct(self, pragma):
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'task', 'target', 'teams']
        
        line = pragma.decode().lstrip().lower()
        clauses = parse_openmp_pragma(line)
        clauses_key = [clause for clause, _ in clauses]

        return '_'.join(list(filter(lambda clause: clause in rel_constrcuts, clauses_key)))

    def reorder_pragma(self, pragma_lst):
        rel_constrcuts = ['do' if self.lang == 'fortran' else 'for', 'sections', 'target', 'task', 'teams']

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
                
    def replace_pragma(self, mappings):
        result = []

        for code, pragma in self.samples:
            # print(code, pragma)

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
                # print(mappings)
                # print('='*20)
                # print(code)
                # print('='*20)
                # print( pragma)
                # print('='*20)
                # print(original_code)
                # print('='*20)
                # print(replaced_code)
                # print('='*20)
                print(e)
                # exit()
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

            if self.lang == 'fortran':
                if not code.startswith('subroutine') and not code.startswith('function'):
                    return []
                
                node = parse(code.lower(), lang=self.lang).root_node
                self.detect_omp_fortran(node)
            else:
                node = parse(code, lang=self.lang).root_node
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

        batch_size = 10000

        def parse_json(json_file):             
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                data = f.readlines()
                dataset = []
                idx = int(sys.argv[1])

                for line in tqdm(data[idx*batch_size:(idx+1)*batch_size]):

                    js = json.loads(line.strip())
                    samples = self.extractor.extract_openmp(js['code'].strip())

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
                            # print(js['code'])
                            # print(target)
                            # print(loop)
                            dataset.append({'code': loop,
                                            'pragma': target,
                                            'hash': preprocess.get_hash(loop)
                            })
                    
                    self.extractor.reset()

                print(counter)
                # write the dataset into json
                with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{idx}.jsonl"), 'w') as data_f:
                    for sample in dataset:
                        data_f.write(json.dumps(sample) + '\n')                
        
        files = os.listdir(self.data_dir)

        for file in tqdm(files):
            parse_json(file)


parser = DatasetCreatorOMP('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran', '/home/1010/talkad/Downloads/OMP_Dataset/fortran/replaced', lang='fortran', replaced=True)
parser.iterate_corpus()








# code = '''subroutine incell2(ni,r2,r1)                                                                            | 747/10000 [00:06<01:18, 118.17it/s]









#       implicit none
#       integer ni
#       real*8  r2(3,*),r1(3,*)



#       real*8  fa1,fa2,fa3
#       real*8  a(3,3),b(3,3),volume
#       integer i,j



#        real*8   lattice_unita
#        external lattice_unita



#       do j=1,3
#       do i=1,3
#         a(i,j) = lattice_unita(i,j)
#       end do
#       end do

#       b(1,1) = a(2,2)*a(3,3) - a(3,2)*a(2,3)
#       b(2,1) = a(3,2)*a(1,3) - a(1,2)*a(3,3)
#       b(3,1) = a(1,2)*a(2,3) - a(2,2)*a(1,3)
#       b(1,2) = a(2,3)*a(3,1) - a(3,3)*a(2,1)
#       b(2,2) = a(3,3)*a(1,1) - a(1,3)*a(3,1)
#       b(3,2) = a(1,3)*a(2,1) - a(2,3)*a(1,1)
#       b(1,3) = a(2,1)*a(3,2) - a(3,1)*a(2,2)
#       b(2,3) = a(3,1)*a(1,2) - a(1,1)*a(3,2)
#       b(3,3) = a(1,1)*a(2,2) - a(2,1)*a(1,2)
#       volume = a(1,1)*b(1,1)
#      >       + a(2,1)*b(2,1)
#      >       + a(3,1)*b(3,1)

#       volume = 1.0d0/volume
#       call dscal(9,volume,b,1)

# !$OMP DO
#       do i =1,ni



#          fa1 =  b(1,1) * r2(1,i)
#      >       +  b(2,1) * r2(2,i)
#      >       +  b(3,1) * r2(3,i)

#          fa2 =  b(1,2) * r2(1,i)
#      >       +  b(2,2) * r2(2,i)
#      >       +  b(3,2) * r2(3,i)

#          fa3 =  b(1,3) * r2(1,i)
#      >       +  b(2,3) * r2(2,i)
#      >       +  b(3,3) * r2(3,i)







#    23   IF (fa1 .GT. (0.5d0)) THEN


#            r2(1,i) = r2(1,i) - lattice_unita(1,1)
#            r2(2,i) = r2(2,i) - lattice_unita(2,1)
#            r2(3,i) = r2(3,i) - lattice_unita(3,1)

#            r1(1,i) = r1(1,i) - lattice_unita(1,1)
#            r1(2,i) = r1(2,i) - lattice_unita(2,1)
#            r1(3,i) = r1(3,i) - lattice_unita(3,1)


#            fa1 =  b(1,1) * r2(1,i)
#      >         +  b(2,1) * r2(2,i)
#      >         +  b(3,1) * r2(3,i)
#            GO TO 23
#         ENDIF
   
#    24   IF (fa1 .LE. (-0.5d0)) THEN


#            r2(1,i) = r2(1,i) + lattice_unita(1,1)
#            r2(2,i) = r2(2,i) + lattice_unita(2,1)
#            r2(3,i) = r2(3,i) + lattice_unita(3,1)

#            r1(1,i) = r1(1,i) + lattice_unita(1,1)
#            r1(2,i) = r1(2,i) + lattice_unita(2,1)
#            r1(3,i) = r1(3,i) + lattice_unita(3,1)


#            fa1 =  b(1,1) * r2(1,i)
#      >         +  b(2,1) * r2(2,i)
#      >         +  b(3,1) * r2(3,i)
#             GO TO 24
#         ENDIF

#    25   IF (fa2 .GT. (0.5d0)) THEN


#            r2(1,i) = r2(1,i) - lattice_unita(1,2)
#            r2(2,i) = r2(2,i) - lattice_unita(2,2)
#            r2(3,i) = r2(3,i) - lattice_unita(3,2)

#            r1(1,i) = r1(1,i) - lattice_unita(1,2)
#            r1(2,i) = r1(2,i) - lattice_unita(2,2)
#            r1(3,i) = r1(3,i) - lattice_unita(3,2)


#            fa2 =  b(1,2) * r2(1,i)
#      >         +  b(2,2) * r2(2,i)
#      >         +  b(3,2) * r2(3,i)
#           GO TO 25
#         ENDIF
   
#    26   IF (fa2 .LE. (-0.5d0)) THEN


#            r2(1,i) = r2(1,i) + lattice_unita(1,2)
#            r2(2,i) = r2(2,i) + lattice_unita(2,2)
#            r2(3,i) = r2(3,i) + lattice_unita(3,2)

#            r1(1,i) = r1(1,i) + lattice_unita(1,2)
#            r1(2,i) = r1(2,i) + lattice_unita(2,2)
#            r1(3,i) = r1(3,i) + lattice_unita(3,2)


#            fa2 =  b(1,2) * r2(1,i)
#      >         +  b(2,2) * r2(2,i)
#      >         +  b(3,2) * r2(3,i)
#            GO TO 26
#         ENDIF


#    27   IF (fa3 .GT. (0.5d0)) THEN


#           r2(1,i) = r2(1,i) - lattice_unita(1,3)
#           r2(2,i) = r2(2,i) - lattice_unita(2,3)
#           r2(3,i) = r2(3,i) - lattice_unita(3,3)

#           r1(1,i) = r1(1,i) - lattice_unita(1,3)
#           r1(2,i) = r1(2,i) - lattice_unita(2,3)
#           r1(3,i) = r1(3,i) - lattice_unita(3,3)

#           fa3 =  b(1,3) * r2(1,i)
#      >        +  b(2,3) * r2(2,i)
#      >        +  b(3,3) * r2(3,i)
#            GO TO 27
#         ENDIF
  
#    28   IF (fa3 .LE. (-0.5d0)) THEN


#           r2(1,i) = r2(1,i) + lattice_unita(1,3)
#           r2(2,i) = r2(2,i) + lattice_unita(2,3)
#           r2(3,i) = r2(3,i) + lattice_unita(3,3)

#           r1(1,i) = r1(1,i) + lattice_unita(1,3)
#           r1(2,i) = r1(2,i) + lattice_unita(2,3)
#           r1(3,i) = r1(3,i) + lattice_unita(3,3)

#           fa3 =  b(1,3) * r2(1,i)
#      >        +  b(2,3) * r2(2,i)
#      >        +  b(3,3) * r2(3,i)
#            GO TO 28
#         ENDIF

#       end do 
# !$OMP END DO

#       return
#       end
# '''

# extractor = OMP_Extractor(lang='fortran', replaced=True)
# print(extractor.extract_openmp(code))
