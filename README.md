# OMPify: Automated Conversion from Serial to Shared-Memory Parallelization

The full OMPify paper can be found [here](https://arxiv.org/abs/2305.11999).

There is an ever-present need for shared memory parallelization schemes to exploit the full potential of multi-core architectures.
The most common parallelization API addressing this need today is OpenMP. Nevertheless, writing parallel code manually is complex and
effort-intensive. Thus, many deterministic source-to-source (S2S) compilers have emerged, intending to automate the process of translating serial
to parallel code. However, recent studies have shown that these compilers are impractical in many scenarios. In this work, we combine the
latest advancements in the field of AI and natural language processing (NLP) with the vast amount of open-source code to address the problem
of automatic parallelization. Specifically, we propose a novel approach, called OMPify, to detect and predict the OpenMP pragmas and shared-
memory attributes in parallel code, given its serial version. OMPify is based on a Transformer-based model that leverages a graph-based representation of source code that exploits the inherent structure of code. We evaluated our tool by predicting the parallelization pragmas and attributes of a large corpus of (over 54,000) snippets of serial code written in C and C++ languages (Open-OMP-Plus). Our results demonstrate that OMPify outperforms existing approaches - the general-purposed and popular ChatGPT and targeted PragFormer models - in terms of F1 score and accuracy. Specifically, OMPify achieves up to 90% accuracy on commonly-used OpenMP benchmark tests such as NAS, SPEC, and PolyBench. Additionally, we performed an ablation study to assess the impact of different model components and present interesting insights derived from the study. Lastly, we also explored the potential of using data augmentation and curriculum learning techniques to improve the model's robustness and generalization capabilities.

In this repository,  you will find the dataset and source code required to reproduce the results we obtained.

<p align="center">
  <img src="https://github.com/talkad/MAANE/assets/48416212/5b1c5113-61b5-4785-a75b-008788219a50" alt=""/>
</p>

## All Publications
  1. [Advising OpenMP Parallelization via a Graph-Based Approach with Transformers
](https://arxiv.org/abs/2305.11999)
  2. [Quantifying OpenMP: Statistical Insights into Usage and Adoption](https://arxiv.org/abs/2308.08002)
  3. [Scope is all you need: Transforming LLMs for HPC Code](https://arxiv.org/abs/2308.09440)
  4. [Domain-Specific Code Language Models: Unraveling the Potential for HPC Codes and Tasks]()

