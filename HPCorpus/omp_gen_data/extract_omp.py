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
                node = parse(code.lower(), lang=self.lang).root_node
                self.detect_omp_fortran(node)
            else:
                node = parse(code, lang=self.lang).root_node
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
                dataset = []
                idx = 5
                for line in tqdm(data[idx*10000:(idx+1)*10000]):

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


            # with open(os.path.join(self.data_dir, json_file), 'r') as f:
            #     data = f.readlines()
                
            #     for idx, start_index in enumerate(range(0, N, batch_size)):
            #         dataset = []
            #         end_index = start_index + batch_size
            #         print(f'current range {start_index}-{end_index}')
                
            #         for line in tqdm(data[start_index:end_index]):

            #             js = json.loads(line.strip())
            #             samples = self.extractor.extract_openmp(js['code'])

            #             for loop, target in samples:
                            
            #                 if target:
            #                     ### count ###
            #                     clauses = parse_openmp_pragma(target)
            #                     clauses_key = [clause for clause, _ in clauses]

            #                     for k,v in counter.items():
            #                         if k in clauses_key:
            #                             counter[k]['_'] += 1

            #                         for vv in v:
            #                             if k in clauses_key and vv in clauses_key:
            #                                 counter[k][vv] += 1
            #                     #############
            #                     # print(target)
            #                     # print(loop)
            #                     dataset.append({'code': loop,
            #                                     'pragma': target,
            #                                     'hash': preprocess.get_hash(loop)
            #                     })
            #                     # print(counter)
            #                     # return
                        
            #             self.extractor.reset()

            #         print(counter)
            #         # write the dataset into json
            #         with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{idx}.jsonl"), 'w') as data_f:
            #             for sample in dataset:
            #                 data_f.write(json.dumps(sample) + '\n')
                
        
        files = os.listdir(self.data_dir)

        for file in tqdm(files):
            parse_json(file)


# parser = DatasetCreatorOMP('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran', '/home/1010/talkad/Downloads/OMP_Dataset/fortran', lang='fortran')
# parser.iterate_corpus()




# code = '''
#  integer, dimension(3)                                     ::  sz
#     real, dimension(1-g:sz(1)+g, 1-g:sz(2)+g, 1-g:sz(3)+g)    ::  p
#     integer, dimension(0:5)                                   ::  nID

#     if ( nID(m_face) >= 0 ) return

#     ix = sz(1)
#     jx = sz(2)
#     kx = sz(3)
#     face = m_face

# !$OMP PARALLEL &
# !$OMP FIRSTPRIVATE(ix, jx, kx, face)

#     FACES : select case (face)
#     case (X_minus)

# !$OMP DO &
# !$omp  SCHEDULE(static) &
# !$OMP  PRIVATE(j, k)
#       do k=1,kx
#       do j=1,jx
#         p(0,j,k) = p(1,j,k)
#       end do
#       end do
# !$OMP END DO
      
#     case (X_plus)

# !$OMP DO SCHEDULE(static) PRIVATE(j, k)
#       do k=1,kx
#       do j=1,jx
#         p(ix+1,j,k) = p(ix,j,k)
#       end do
#       end do
# !$OMP END DO
      
#     case (Y_minus)

# !$OMP DO SCHEDULE(static) PRIVATE(i, k)
#       do k=1,kx
#       do i=1,ix
#         p(i,0,k) = p(i,1,k)
#       end do
#       end do
# !$OMP END DO
      
#     case (Y_plus)

# !$OMP DO SCHEDULE(static) PRIVATE(i, k)
#       do k=1,kx
#       do i=1,ix
#         p(i,jx+1,k) = p(i,jx,k)
#       end do
#       end do
# !$OMP END DO
      
#     case (Z_minus)

# !$OMP DO SCHEDULE(static) PRIVATE(i, j)
#       do j=1,jx
#       do i=1,ix
#         p(i,j,0) = p(i,j,1)
#       end do
#       end do
# !$OMP END DO
    
#     case (Z_plus)

# !$OMP DO SCHEDULE(static) PRIVATE(i, j)
#       do j=1,jx
#       do i=1,ix
#         p(i,j,kx+1) = p(i,j,kx)
#       end do
#       end do
# !$OMP END DO
      
#     case default
#     end select FACES

# !$OMP END PARALLEL

#     return
#     end subroutine pobc_neumann
# '''

extractor = OMP_Extractor(lang='fortran')
# print(extractor.extract_openmp(code))

print(extractor.get_pragma('omp do private(ipol  n3  n2  n1  bgtau  arg)'))
