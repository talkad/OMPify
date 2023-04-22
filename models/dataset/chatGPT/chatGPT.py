import json

with open('chatGPT.txt', 'w') as chatgpt, open('gpt_lables.jsonl', 'w') as labels, open('./test.jsonl', 'r') as spec:
    for idx, line in enumerate(spec):
        js = json.loads(line.lstrip())
        code = js['code']
        if len(code.split('\n')) < 15:
            chatgpt.write(f'{idx}. {code}\n')
            labels.write(json.dumps({'idx': idx,
                                         'exist': js['exist']})+'\n')
