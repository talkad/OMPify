import re
from parse_tools import parse
import preprocess


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
    def __init__(self, lang):
        self.lang = lang

        self.samples = []

    def reset(self):
        self.sample = []

    def is_omp_for_pragma(self, pragma):
        '''
        Returns true if the given pragma is omp-for
        '''
        line = pragma.lstrip().lower()
        clauses = [clause for clause, _ in parse_openmp_pragma(line)]

        return all([clause in clauses for clause in ['omp', 'for']])

    def detect_for(self, node):
        children = node.children
        num_children = len(children)

        pragma = ''

        for child in children:
            if child.type == 'preproc_call' and self.is_omp_for_pragma(child.text):
                pragma = child.text
            elif len(child.type) == 1:
                pass
            elif pragma and child.type in ['compound_statement', 'for_statement']:
                self.samples.append((child.text.decode(), pragma.decode()))
                pragma = ''
            elif not pragma and child.type == 'for_statement':
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






extractor = OMP_Extractor(lang='c')


code = """
int main() {
    int num_threads = 4;

    omp_set_num_threads(num_threads);

    int n = 16; 

    #pragma omp target teams distribute parallel for simd
    for (int i = 0; i < n; i++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d: Iteration %d\n", thread_id, i);
    }


    #pragma omp parallel for num_threads(4)
    {
        printf("hello world");
    }

    while(1) {
    #pragma omp parallel for target 
     for (int i = 0; i < n; i++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d: Iteration %d\n", thread_id, i);
        }
    }

    return 0;
}
"""

for sample in extractor.extract_for_loops(code):
    print(sample)