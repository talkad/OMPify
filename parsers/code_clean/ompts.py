import re

redundant_ompts = re.compile("<ompts:testdescription>.*<\/ompts:testdescription>|<ompts:ompversion>.*<\/ompts:ompversion>|<ompts:directive>.*<\/ompts:directive>|<ompts:dependences>.*<\/ompts:dependences>|<ompts:.*?>|<\/ompts:.*>")

with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/omp_for_schedule_static_3.c', 'r') as f:
    code = f.read()

code = redundant_ompts.sub("", code)
print(code)