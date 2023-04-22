# Scaling Up: Automated Conversion from Serial to Shared-Memory Parallelization

In order to exploit the full potential of multi-core architectures, which are fundamental components of modern computing, there is an ever-present need for shared memory parallelization schemes. Nowadays, the most common parallelization API addressing this task is OpenMP. Although this API is comprehensive, code parallelization remains a challenging task.

To address this challenge, many source-to-source (S2S) compilers have emerged in recent years, attempting to automate the process. However, these compilers have limitations, such as being time-consuming and having limited robustness to inputs.

To tackle these limitations, we will use Language Models to effectively learn the semantics of a code. We present a tool that enables the creation of Open-OMP, a database containing code files that use Open-MP pragmas, extracted for-loops from the code, their AST format, and the Open-MP pragma related to the for-loop (if it exists).

Our for-loop extractor currently supports the C, C++ (plain code and their LLVM-IR format), and Fortran programming languages, and it can be extended by inheriting from the Parser abstract class.


## General Architecture

![image](https://user-images.githubusercontent.com/48416212/225433045-3219d663-9244-4274-ac60-f897317c68d5.png)


## Future Plans

We plan to evaluate the effectiveness of various code modalities, such as plain code, Abstract Syntax Trees (ASTs), and Intermediate Representations (IRs), on different Language Models.

## Initial Results

We assessed multiple Language Models across various modalities (Plain Code, AST, IR) for their effectiveness in classifying whether a for loop could be parallelized using an OpenMP pragma. The following table contatins the results:

<table>
    <thead>
        <tr>
            <th>Model</th>
            <th>Modalities</th>
            <th>Accuracy</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=2>CodeBERT</td>
            <td>Plain Code</td>
            <td>86.2%</td>
        </tr>
        <tr>
            <td>LLVM IR</td>
            <td>82.5%</td>
        </tr>
        <tr>
            <td rowspan=2>SPT-Code</td>
            <td>Plain Code</td>
            <td>84.7%</td>
        </tr>
        <tr>
            <td>Plain Code + AST</td>
            <td>84.8%</td>
        </tr>
    </tbody>
</table>



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

