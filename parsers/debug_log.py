with open('/home/talkad/OpenMPdb/parsers/cbe_error.txt', 'r') as f:
    lines = f.readlines()
    types = set()

    for idx, line in enumerate(lines):
        if '_fake_typedefs.h' in line:
            types.add(lines[idx+1])

    for a in types:
        print(a)
