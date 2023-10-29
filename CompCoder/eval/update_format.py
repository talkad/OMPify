file = '/home/talkad/Downloads/omp_generation_outputs.txt'

def update_pragma(pragma):
    return pragma.replace('(', ' ( ').replace(')', ' ) ').replace(':', ' : ')

with open(file, "r") as f:
    lines = f.readlines()


with open("polycoder_c.log", "w") as outfile:
    i = 0
    while i+2 < len(lines):
        line1 = lines[i].strip()  
        i += 2
        line2 = lines[i].strip()  
        i += 3

        outfile.write(f"| {update_pragma(line1)} | {update_pragma(line2)} | \n")

