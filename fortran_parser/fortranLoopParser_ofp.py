from open_fortran_parser import parse
import pathlib
import tempfile
import xml.etree.ElementTree as ET
import pickle
import os

def is_for_pragma(line):
    '''
    Returns true if the given line is an OMP pragma

    Parameters:
        line (str) - a single line from source coded 
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    return sub_line.startswith('!$omp ') and ' do' in line and ' end' not in line

def is_fortran_comment(line):
    '''
    A line with a c, C, or ! in column one is a comment line

    Parameters:
        line (str) - a single line from source coded 
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    return line[0] == 'c' or \
        (sub_line[0] == '!' and not (len(sub_line) > 1 and sub_line[1] == '$'))


class OmpLoop:
    def __init__(self, omp_pragma, loop):
        self.omp_pragma = omp_pragma # omp pragma associated with the given loop
        self.loop = loop             # xml format representing AST structure of loop


class FortranLoopParser:
    def __init__(self, omp_repo, xml_repo):
        self.fortran_extensions = ['.f', '.f90']
        self.root_dir = os.getcwd()
        self.omp_repo = omp_repo
        self.xml_repo = xml_repo
        self.split_idx = len(os.path.join(self.root_dir, self.omp_repo)) + 1


    def create_directory(self, dirs):
        '''
        Create directories if not exists
        '''
        if not os.path.exists(dirs):
            os.makedirs(dirs)

    def save(self, file_path, pragma, loop):
        '''
        Save OmpLoop structure into file_path
        '''
        parsed_loop = OmpLoop(pragma, loop)

        with open(file_path, 'wb') as f:
            pickle.dump(parsed_loop, f)

    def load(self, file_path):
        '''
        Load OmpLoop structure from file_path
        '''
        with open(file_path, 'rb') as f:
            return pickle.load(f)

    def create_xml_tree(self, tmp_filepath, original_filepath):
        try:
            xml_tree = parse(pathlib.Path(tmp_filepath))
            return ET.ElementTree(xml_tree)
        except ET.ParseError:  
            print(f'Invalid file: {original_filepath}')
        except:
            print('Unrecognized error')
            
    def extract_loops(self, root_dir, file_name, root):
        '''
        Extract loops from xml_tree recursively

        note - loops are not in the compound constructs list so inner loops are associated with the top-most loop
        '''
        compound_constructs = ['file', 'subroutine', 'body', 'program', 'specification', 'if']
        save_dir = os.path.join(self.xml_repo, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        pos_count = 0
        neg_count = 0
        omp_loop = False 
        omp_pragma = '' 

        for elm in root:  # root.findall('.//')
            if elm.tag in compound_constructs:
                pos, neg = self.extract_loops(root_dir, file_name, elm)  # recursive call
                pos_count += pos
                neg_count += neg

            elif not omp_loop and elm.tag == 'loop':         # negative example
                self.create_directory(save_dir)              # preserve the file structure of original DB
                self.save(os.path.join(save_dir, f'{name}_neg_{neg_count}.pickle'), None, elm)
                neg_count += 1

            elif elm.tag == 'comment':
                line = elm.attrib['text'].lower()

                if not is_fortran_comment(line) and is_for_pragma(line):
                    omp_pragma = line
                    omp_loop = True

            elif omp_loop and elm.tag == 'loop':              #positive example
                self.create_directory(save_dir) 
                self.save(os.path.join(save_dir, f'{name}_pos_{pos_count}.pickle'), omp_pragma, elm)
                pos_count += 1
                omp_loop = False

        return pos_count, neg_count
    
    def parse_file(self, root_dir, file_name):
        '''
        Parse the given file into xml format and extract to loops associated with omp pargma
        '''
        file_path = os.path.join(root_dir, file_name)
        
        with open(file_path, 'rb') as f:

            try:
                code = f.read().decode('utf-8')
            except UnicodeDecodeError:
                print(f'Decode error: {file_path}')
                return None, None

            with tempfile.NamedTemporaryFile('w+') as tmp:
                print(code, file=tmp, flush=True)

                xml_tree = self.create_xml_tree(tmp.name, file_path)
                if xml_tree is None:
                    return None, None

                root = xml_tree.getroot()

                with open('example.xml', 'wb') as a:
                    ET.ElementTree(root).write(a)

                return self.extract_loops(root_dir, file_name, root)
                

    def scan_dir(self):
        # check if fortran file
        total_pos = 0
        total_neg = 0
        omp_repo = os.path.join(self.root_dir, self.omp_repo)

        for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()
                
                if ext in self.fortran_extensions:
                    pos, neg = self.parse_file(root, file_name)

                    if pos is not None:
                        total_pos += pos
                        total_neg += neg

            if idx % (10**2) == 0:
                print(f'Message {idx // (10**2)}) {total_pos} {total_neg}')

        return total_pos, total_neg


# parser = FortranLoopParser('repositories_openMP', 'fortran_loops')
parser = FortranLoopParser('../123', 'fortran_eample')

# parser.parse_file('example.f90')
data = parser.load('fortran_eample/area_neg_0.pickle')
with open('example2.xml', 'wb') as a:
    ET.ElementTree(data.loop).write(a)
# print(data.omp_pragma)
# ET.dump(data.loop)

print(ET.tostring(data.loop, encoding='unicode'))

# total = parser.scan_dir()
# print(total)

# pos - 3564 
# neg - 2497 