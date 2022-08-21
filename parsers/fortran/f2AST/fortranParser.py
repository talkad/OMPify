from open_fortran_parser import parse
import pathlib
import tempfile
import xml.etree.ElementTree as ET

with open('/home/talkad/Downloads/thesis/data_gathering_script/parsers/fortran/f2AST/example.f90', 'rb') as f:
    code = f.read().decode('utf-8')

    with tempfile.NamedTemporaryFile('w+') as tmp:
        print(code, file=tmp, flush=True)
        xml_tree = parse(pathlib.Path(tmp.name))
        for i, loop in enumerate(xml_tree.iter('loop')):
            print(f'loop num {i}')
            ET.dump(loop)
        # ET.dump(xml_tree)