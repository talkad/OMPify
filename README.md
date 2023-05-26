# OMPify: Automated Conversion from Serial to Shared-Memory Parallelization

The full paper can be found [here](https://arxiv.org/abs/2305.11999).

There is an ever-present need for shared memory parallelization schemes to exploit the full potential of multi-core architectures.
The most common parallelization API addressing this need today is OpenMP. Nevertheless, writing parallel code manually is complex and
effort-intensive. Thus, many deterministic source-to-source (S2S) compilers have emerged, intending to automate the process of translating serial
to parallel code. However, recent studies have shown that these compilers are impractical in many scenarios. In this work, we combine the
latest advancements in the field of AI and natural language processing (NLP) with the vast amount of open-source code to address the problem
of automatic parallelization. Specifically, we propose a novel approach, called OMPify, to detect and predict the OpenMP pragmas and shared-
memory attributes in parallel code, given its serial version. OMPify is based on a Transformer-based model that leverages a graph-based representation of source code that exploits the inherent structure of code. We evaluated our tool by predicting the parallelization pragmas and attributes of a large corpus of (over 54,000) snippets of serial code written in C and C++ languages (Open-OMP-Plus). Our results demonstrate that OMPify outperforms existing approaches - the general-purposed and popular ChatGPT and targeted PragFormer models - in terms of F1 score and accuracy. Specifically, OMPify achieves up to 90% accuracy on commonly-used OpenMP benchmark tests such as NAS, SPEC, and PolyBench. Additionally, we performed an ablation study to assess the impact of different model components and present interesting insights derived from the study. Lastly, we also explored the potential of using data augmentation and curriculum learning techniques to improve the model's robustness and generalization capabilities.

In this repository,  you will find the dataset and source code required to reproduce the results we obtained.


## Overview

<p align="center">
  <img src="https://github.com/talkad/MAANE/assets/48416212/5b1c5113-61b5-4785-a75b-008788219a50" alt=""/>
</p>

## Results

We compared our model, namely OMPify, with the baseline model Pragformer, as well as with ChatGPT. The results are as follows:

### SPEC Benchmark
<table>
   <thead>
      <tr>
         <th>Model</th>
         <th>Precision</th>
         <th>Recall</th>
         <th>F1</th>
         <th>Accuracy</th>
      </tr>
   </thead>
   <tbody>
      <tr>
         <td>PragFormer</td>
         <td>0.445</td>
         <td>0.802</td>
         <td>0.572</td>
         <td>0.837</td>
      </tr>
      <tr>
         <td>OMPify</td>
         <td><strong>0.572</strong></td>
         <td><strong>0.854</strong></td>
         <td><strong>0.685</strong></td>
         <td><strong>0.894</strong></td>
      </tr>
   </tbody>
</table>

### PolyBench Benchmark

<table>
   <thead>
      <tr>
         <th>Model</th>
         <th>Precision</th>
         <th>Recall</th>
         <th>F1</th>
         <th>Accuracy</th>
      </tr>
   </thead>
   <tbody>
      <tr>
         <td>PragFormer</td>
         <td>0.703</td>
         <td>0.301</td>
         <td>0.422</td>
         <td>0.648</td>
      </tr>
      <tr>
         <td>OMPify</td>
         <td><strong>0.836</strong></td>
         <td><strong>0.810</strong></td>
         <td><strong>0.823</strong></td>
         <td><strong>0.851</strong></td>
      </tr>
   </tbody>
</table>

### NAS Benchmark

<table>
   <thead>
      <tr>
         <th>Model</th>
         <th>Precision</th>
         <th>Recall</th>
         <th>F1</th>
         <th>Accuracy</th>
      </tr>
   </thead>
   <tbody>
      <tr>
         <td>PragFormer</td>
         <td>0.635</td>
         <td>0.734</td>
         <td>0.681</td>
         <td>0.634</td>
      </tr>
      <tr>
         <td>OMPify</td>
         <td><strong>0.731</strong></td>
         <td><strong>0.886</strong></td>
         <td><strong>0.801</strong></td>
         <td><strong>0.766</strong></td>
      </tr>
   </tbody>
</table>

### ChatGPT Test

<table>
   <thead>
      <tr>
         <th>Model</th>
         <th>Precision</th>
         <th>Recall</th>
         <th>F1</th>
         <th>Accuracy</th>
      </tr>
   </thead>
  <tbody>
    <tr>
      <td>ChatGPT</td>
      <td>0.401</td>
      <td><strong>0.913</strong></td>
      <td>0.557</td>
      <td>0.401</td>
    </tr>
    <tr>
      <td>PragFormer</td>
      <td>0.8153</td>
      <td>0.7215</td>
      <td>0.7655</td>
      <td>0.8176</td>
    </tr>
    <tr>
      <td>OMPify</td>
      <td><strong>0.839</strong></td>
      <td>0.818</td>
      <td><strong>0.828</strong></td>
      <td><strong>0.860</strong></td>
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
