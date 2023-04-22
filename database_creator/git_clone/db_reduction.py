from parsers.cParser import CLoopParser
import random
import os
import json

class DB_Reducer:
    '''
    The majority of our database are negative examples - loops without pragma
    we will remove certain amount of the negative examples in the following manner:
        1. for each code file, we will count the number of positive examples (num_pos)
        2. we will keep num_pos negative examples from the same file
            2.1. if for a given file num_pos > num_neg, we will keep extra negative examples from the next file
    '''

    def __init__(self, file_path):
        self.num_pos = 0
        self.num_neg = 0
        self.parser = CLoopParser('', '')
        self.is_extra = False
        self.original_db = {}
        self.updated_db = {}

        self.load_db(file_path)

    def load_db(self, file_path):
        with open(file_path, 'r') as f:
            self.original_db = json.load(f)

    def extract_files(self):
        '''
        Return for each file a list of all example indexes
        '''
        all_files = {}

        for path in self.original_db.values():
            splitted_path = path.split('/')
            main_path = '/'.join(splitted_path[:-1])

            if main_path in all_files:
                all_files[main_path].append(splitted_path[-1])
            else:
                all_files[main_path] = [splitted_path[-1]]

        return all_files

    def reduce_file(self, file_path, examples):
        '''
        Return a list of all example indexes we will keep
        '''
        pos_examples = []

        for example in examples:
            data = self.parser.load(os.path.join(file_path, example))

            if data.omp_pragma is not None:
                pos_examples.append(example)

        neg_examples = [example for example in examples if example not in pos_examples]
        num_pos = len(pos_examples)+20 if self.is_extra else len(pos_examples)
        neg_examples = random.sample(neg_examples, k=min(num_pos, len(neg_examples)))
        self.is_extra = True if len(pos_examples)*2 > len(examples) else False

        self.num_pos += len(pos_examples)
        self.num_neg += len(neg_examples)

        return pos_examples + neg_examples
    
    def reduce_files(self):
        '''
        updates the new db, and dumps it into a json file
        '''
        all_files = self.extract_files()
        idx = 0

        for file_path, examples in all_files.items():
            reduced_examples = self.reduce_file(file_path, examples)

            for example in reduced_examples:
                self.updated_db[str(idx)] = os.path.join(file_path, example)
                idx += 1

        with open("updated_db.json", "w") as f:
            json.dump(self.updated_db, f, indent=4, separators=(',', ': '))



reducer = DB_Reducer('/home/talkad/Downloads/thesis/data_gathering_script/sample.json')
reducer.reduce_files()

print(f'neg {reducer.num_neg}\npos {reducer.num_pos}')
        



