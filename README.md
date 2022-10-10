# Open-OMP Creator

Since 2016, numerous Github repositories have made extensive use of Open-MP [[1]](https://user-images.githubusercontent.com/48416212/188301399-076375b3-b408-4d41-b40a-99d2f6eebf9b.png).  This use can be leveraged for solving a wide range of tasks, such as trending analysis, caluses calssification, omp pragmas generation and many more.

This tool enables the creation of Open-OMP, which is a database contains code files that are using Open-MP pragmas, the extracted for-loops from the code, their AST format and the Open-MP pragma related to the for-loop (if exists). 

Currently our for-loop extractor supports the C, C++ and Fortran programming languages, but it can be extended by inherit from the Parser abstract class.

## Examples

1. Load repositories from github between range of dates and extract all the for-loops from C and C++ programming languages:
```
python main.py --load "1-5-2008..10-8-2022" --parse "(c|cpp)"
```

2. Show Open-MP trends in github:
```
python main.py --stats
```
output:
![image](https://user-images.githubusercontent.com/48416212/194878405-4261c503-328b-46b1-883c-5b8dda06d7a3.png)

