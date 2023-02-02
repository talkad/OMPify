import json

# files = {}
# max_id = 0

# with open("sample_c.json", "r") as f:
#     c_files_old = json.load(f)

# for k, v in c_files_old.items():
#     files[int(k)] = v
#     max_id = k

# max_id = int(max_id) + 1

# with open("sample_cpp.json", "r") as f:
#     cpp_files_old = json.load(f)

# for k, v in cpp_files_old.items():
#     files[int(k) + max_id] = v

# with open("sample_c_cpp.json", "w") as f:
#     json.dump(files, f, sort_keys=True, indent=4, separators=(',', ': '))

with open("sample_c_cpp.json", "r") as f:
    files = json.load(f)
    print(len(files))