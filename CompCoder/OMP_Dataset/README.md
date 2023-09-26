# High-Performance Computing (HPCorpus) OpenMP For-Loops Dataset

## Overview
This dataset contains a collection of unique for-loops extracted from the [HPCorpus](https://github.com/Scientific-Computing-Lab-NRCN/HPCorpus) that utilize OpenMP directives. The dataset encompasses for-loops written in the C, C++, and Fortran programming languages, and each for-loop sample is structured with specific fields, including the code snippet and the associated OpenMP pragma.

## Dataset Structure
The dataset is organized into three main programming language folders: C, C++, and Fortran. 
Each language folder is further divided into two subfolders: "source" and "replaced." Here's the hierarchical structure of the dataset:

|-- c \
|--|-- source: Original for-loops written in C. \
|--|-- replaced: Replaced for-loops with statically generated variable names. \
|-- cpp \
|--|-- source: Original for-loops written in C++. \
|--|-- replaced: Replaced for-loops with statically generated variable names. \
|-- fortran \
|--|-- source: Original for-loops written in Fortran. \
|--|-- replaced: Replaced for-loops with statically generated variable names. 

## Data Fields
Each sample in the dataset consists of the following fields:

code: This field contains the actual for-loop code snippet as extracted from the HPCorpus.

pragma: The "pragma" field contains the OpenMP directives associated with the for-loop. These directives include the clauses "private," "reduction," and "simd." 
The clauses and the variables inside them are presented in the following order:

1. Clauses are listed alphabetically.
2. Variables within each clause are sorted alphabetically.

## Example:

### Source:

code:

```c
for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
        // Loop code
    }
}
```

pragma:
```
for private(i j) reduction(+:result) simd
```

### Replaced:

```c
for (int var_202 = num_521; var_202 < var_87; var_202++) {
    for (int var_4 = num_8; var_4 < var_87; var_4++) {
        // Loop code
    }
}
```

pragma:
```
for private(var_202 var_4) reduction(+:var_555) simd
```

## Note on the "Replaced" Version
The "replaced" version of the for-loops is generated statically. Variable names within the code segments are replaced to ensure consistency. This is done because not all variables used in the for-loops are necessarily defined within the extracted code, making dynamic replacement impractical.
