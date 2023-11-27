import json

original_file = 'original.log'
output_file = 'output.log'

with open(original_file, 'r') as f, open(output_file, 'w') as out:
    for line in f:
        sample = json.loads(line.strip())
        label, prediction = sample['label'], sample['prediction']

        result = {}
        result['label'] = label

        if 'omp ' not in prediction:
            result['prediction'] = ''
        else:
            pred = prediction[prediction.find('omp '):]
            pred =  pred[:pred.find('\n')]

            if '`' in pred:
                result['prediction'] = pred[:pred.find('`')]
            else:
                result['prediction'] = pred

        out.write(json.dumps(result) + '\n')
            