import re
import code_tokenize as ctok
# from fortranformat import FortranFormat as ftok
import sys
import json
import os
from typing import List
from transformers import AutoTokenizer, GPT2Tokenizer
from abc import ABC, abstractmethod

# sys.path.extend(['.','/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser'])
sys.path.extend(['.','/homes/talkad/OMPify/database_creator/parsers','/homes/talkad/OMPify/database_creator/parsers/HPCorpus_parser'])

from HPCorpus_parser import parse_tools, preprocess
from HPCorpus_parser import convert_representation as cr
import fortranTokenizer as ftok



class Tokenizer(ABC):

    @abstractmethod
    def tokenize(self, s: str, lang: str = 'c') -> List[str]:
        '''
            convert string into sequence tokens

            Parameters:
                s: str -  input string to be tokenized
            Result:
                convert string into list of tokens
        '''
        pass

    @abstractmethod
    def encode(self, s: str, lang: str = 'c') ->  List[int]:
        '''
            encode given string to ids

            Parameters:
                s: String - input string to be tokenized
                lang: String - programming language of @param:s

            Result:
                list of token ids
        '''
        pass

    @abstractmethod
    def decode(self, t: List[int]) -> str:
        '''
            decode token ids to string

            Parameters:
                t: List[int] -  list of tokens ids

            Results:
                string represents the list of ids

        '''
        pass


class Tokompiler(Tokenizer):
    '''
        Compiler oriented tokenization
    '''

    def __init__(self, vocab_path: str = '/homes/talkad/OMPify/tokenizer/vocabs/tokenizer_vocab/tokompiler_vocab.json'):
        with open(vocab_path, 'r') as f:
            vocab = json.loads(f.read())

        self.encoder = {key:int(val) for key, val in vocab.items()}
        self.decoder = {val:key for key, val in self.encoder.items()}

    def tokenize(self, s: str, lang: str = 'c') -> List[str]:
        if len(s.strip()) == 0:
            return []

        s = cr.generate_replaced(s, num_generator=cr.generate_random_numbers, lang=lang)
        if not s:
            return []   # replacement failed

        if lang == 'fortran':
            s = s.lower()
            tokens = ftok.tokenize(s)    
        else:
            try:
                tokens = list(map(lambda token: token.text, 
                                ctok.tokenize(s, lang=lang, syntax_error="ignore")))
            except Exception as e:
                print('ctok error: ', e)
                return []

        updated_tokens = []
        for token in tokens:
            str_token = token.strip()

            if any([str_token.startswith(prefix) for prefix in cr.replaced_prefixes.values()]):
                updated_tokens += list(str_token.split('_'))
            else:
                updated_tokens.append(str_token)
                  
        return updated_tokens

    def encode(self, s: str, lang: str = 'c') ->  List[int]:
        tokens = self.tokenize(s, lang=lang)
        ids = [self.encoder[token] if token in self.encoder else self.encoder["OOV"] for token in tokens]

        return ids

    def decode(self, t: List[int]) -> str:
        tokens = ' '.join([self.decoder[id] for id in t])

        return tokens


class TokenizerBPE(Tokenizer):
    '''
        GPT2 BPE tokenization
    '''
    def __init__(self):
        self.tokenizer = GPT2Tokenizer.from_pretrained('gpt2')

    def tokenize(self, s: str, lang: str = 'c') -> List[str]:

        s = parse_tools.generate_replaced(s, num_generator=parse_tools.generate_random_numbers, lang=lang)

        tokens = self.tokenizer.tokenize(s)
        updated_tokens = []

        for token in tokens:
            if token.startswith('Ċ') or token.startswith('Ġ'):
                updated_token = token[1:]

                if 'Ċ' in updated_token or 'Ġ' in updated_token:
                    continue
            else:
                updated_token = token

            if updated_token:
                updated_tokens.append(updated_token)

        return updated_tokens

    def encode(self, s: str, lang: str = 'c') ->  List[int]:
        s = parse_tools.generate_replaced(s, num_generator=parse_tools.generate_random_numbers, lang=lang)

        return self.tokenizer.encode(s)

    def decode(self, t: List[int]) -> str:
        return self.tokenizer.decode(t)


class ASTokenizer(Tokenizer):
    '''
        convert AST representation into sequence XSBT
    '''
    def tokenize(self, s: str, lang: str = 'c') -> List[str]:
        ast = cr.code2xsbt(s, lang=lang)
        # ast = cr.code2ast(s, lang=lang)

        ast = ast.split()
        updated_ast = []

        for node in ast:
            if any([node.startswith(prefix) for prefix in cr.replaced_prefixes.values()]):
                updated_ast += node.split('_')
            else:
                updated_ast.append(node)

        return updated_ast

    def encode(self, s: str, lang: str = 'c') ->  List[int]:
        return []

    def decode(self, t: List[int]) -> str:
        return ''
    

class DFGTokenizer(Tokenizer):
    '''
        convert DFG representation into sequence of tokens
    '''
    def tokenize(self, s: str, lang: str = 'c') -> List[str]:
        dfg = cr.code2dfg(s, lang=lang)
        updated_dfg = []

        for cnt in dfg:
            sample = (tuple(cnt[0].split('_')), cnt[1], cnt[2], [tuple(var.split('_')) for var in cnt[3]], cnt[4])
            updated_dfg.append(sample)

        return updated_dfg

    def encode(self, s: str, lang: str = 'c') ->  List[int]:
        return []

    def decode(self, t: List[int]) -> str:
        return ''
    

