from tensorboard.backend.event_processing import event_accumulator

# Path to the directory containing TensorBoard event files
logdir = '/home/1010/talkad/OMPify/outputs/ppl_mass_20230804_070027/runs/mass'

# Initialize the event accumulator
ea = event_accumulator.EventAccumulator(logdir)
ea.Reload()

# Create a CSV file to write the event data
csv_file = open('tensorboard_data.csv', 'w')

# Write CSV header
csv_file.write('step,tag,value\n')

# Iterate through the events and write data to the CSV file
for tag in ea.Tags()['scalars']:
    events = ea.Scalars(tag)
    for event in events:
        csv_file.write(f'{event.step},{tag},{event.value}\n')

# Close the CSV file
csv_file.close()

print('CSV file created: tensorboard_data.csv')