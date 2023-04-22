import numpy as np
import time
import torch
from sklearn.metrics import accuracy_score
from sklearn.metrics import recall_score
from sklearn.metrics import precision_score
from sklearn.metrics import f1_score
from sklearn.metrics import confusion_matrix
from tqdm import tqdm
from torch.utils.data import TensorDataset, DataLoader, RandomSampler, SequentialSampler
from transformers import AutoTokenizer
import os
import json

from replace import create_sample


def deepscc_tokenizer(data, max_len=150, pt_model="/home/talkad/Desktop/pragformer_projects/pragformer/PragFormer/DeepSCC-RoBERTa"):
    tokenizer = AutoTokenizer.from_pretrained(pt_model)

    if max_len == 0:
        tokenized = tokenizer.batch_encode_plus(
            data,
            # max_length = max_len,
            pad_to_max_length=True,
            truncation=True
        )
    else:
        tokenized = tokenizer.batch_encode_plus(
            data,
            max_length = max_len,
            pad_to_max_length = True,
            truncation = True
        )

    return tokenized, tokenizer.vocab_size


def get_data_loader(split, update_ratio=0):

    data_path = '/home/talkad/Downloads/thesis/data_gathering_script/models/dataset/json_c_cpp'
    model_pretained_name = "NTUYG/DeepSCC-RoBERTa"

    df = {}

    df[split] = []
    with open(os.path.join(data_path, f'{split}.jsonl')) as f:
        for line in tqdm(f):
            js = json.loads(line.strip())
            # updated_code, _ = create_sample(js['path'], update_ratio)
            ### DEBUG ###
            df[split].append((js['code'], js['exist']))

    # cls0_samples = [sample for sample in df[split] if sample[1]==0]
    # cls1_samples = [sample for sample in df[split] if sample[1]==1]
    # cls0_samples = cls0_samples[:int(len(cls1_samples)*1.3)]
    # df[split] = cls0_samples + cls1_samples


    train, _ = deepscc_tokenizer([code for code, _ in df[split]], 0, model_pretained_name)
    train_seq = torch.tensor(train['input_ids'])
    train_mask = torch.tensor(train['attention_mask'])
    train_y = torch.tensor([label for _, label in df[split]])

    # wrap tensors
    train_data = TensorDataset(train_seq, train_mask, train_y)
    # sampler for sampling the data during training
    train_sampler = RandomSampler(train_data)
    # dataLoader for train set
    train_dataloader = DataLoader(train_data, sampler = train_sampler, batch_size = 32)

    return train_dataloader



def train(model, epochs, train_dataloader, device, cross_entropy, optimizer, val_dataloader, save):

    val_dataloader = get_data_loader('valid')
    test_nas_dataloader = get_data_loader('test_nas')
    test_poly_dataloader = get_data_loader('test_poly')
    test_spec_dataloader = get_data_loader('test_spec')

    train_losses = []
    valid_losses = []
    # set initial loss to infinite
    best_valid_loss = float('inf')
    result_name = save.split(".pt")[0] + ".txt"
    f = open(result_name, "w")
    f.close()
    # for each epoch
    ratios = [0, 0.05, 0.15, 0.3, 0.4]
    ratios = [0, 0.05, 0.15, 0.3]

    for epoch in range(epochs):
        ratio = ratios[epoch] if epoch<len(ratios) else 0.3
        # ratio = 1
        train_dataloader = get_data_loader('train', ratio)

        print('\n Epoch {:} / {:}'.format(epoch + 1, epochs))
        print(f'updated ratio {ratio}')

        # train model
        train_loss, _ = train_epoch(model, train_dataloader, device, cross_entropy, optimizer)

        # evaluate model
        print('###### valid ######')
        valid_loss, acc_score = evaluate(model, val_dataloader, device, cross_entropy)
        print('###### test nas ######')
        valid_loss, acc_score = evaluate(model, test_nas_dataloader, device, cross_entropy)
        print('###### test poly ######')
        valid_loss, acc_score = evaluate(model, test_poly_dataloader, device, cross_entropy)
        print('###### test spec ######')
        valid_loss, acc_score = evaluate(model, test_spec_dataloader, device, cross_entropy)

        # # save the best model
        # if valid_loss < best_valid_loss:
        #     best_valid_loss = valid_loss

        #     torch.save(model.state_dict(), save)

        # # append training and validation loss
        # train_losses.append(train_loss)
        # valid_losses.append(valid_loss)

        # print(f'\nTraining Loss: {train_loss:.3f}')
        # print(f'Validation Loss: {valid_loss:.3f}')
        # print(f'Accuracy: {acc_score:.3f}')
        # with open(result_name, "a") as f:
        #     f.writelines(f'{train_loss:.3f} {valid_loss:.3f} {acc_score:.3f}')
        #     f.writelines("\n")
        # f = open("results.txt", "a")




def train_epoch(model, train_dataloader, device, cross_entropy, optimizer):
    model.train()

    total_loss, total_accuracy = 0, 0

    # empty list to save model predictions
    total_preds = []
    # iterate over batches
    for step, batch in tqdm(enumerate(train_dataloader)):

        # progress update after every 50 batches.
        if step % 50 == 0 and not step == 0:
            print('  Batch {:>5,}  of  {:>5,}.'.format(step, len(train_dataloader)))

        # push the batch to gpu
        batch = [r.to(device) for r in batch]

        sent_id, mask, labels = batch

        # clear previously calculated gradients
        model.zero_grad()

        # get model predictions for the current batch
        preds = model(sent_id, mask)

        # compute the loss between actual and predicted values
        loss = cross_entropy(preds, labels)

        # add on to the total loss
        total_loss = total_loss + loss.item()

        # backward pass to calculate the gradients
        loss.backward()

        # clip the the gradients to 1.0. It helps in preventing the exploding gradient problem
        torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)

        # update parameters
        optimizer.step()

        # model predictions are stored on GPU. So, push it to CPU
        preds = preds.detach().cpu().numpy()

        # append the model predictions
        total_preds.append(preds)

        # compute the training loss of the epoch
    avg_loss = total_loss / len(train_dataloader)

    # predictions are in the form of (no. of batches, size of batch, no. of classes).
    # reshape the predictions in form of (number of samples, no. of classes)
    total_preds = np.concatenate(total_preds, axis = 0)

    # returns the loss and predictions
    return avg_loss, total_preds



# function for evaluating the model
def evaluate(model, val_dataloader, device, cross_entropy):
    print("\nEvaluating...")
    y_trues = []
    y_preds = []

    # deactivate dropout layers
    model.eval()

    total_loss, total_accuracy = 0, 0
    acc_score = 0
    # empty list to save the model predictions
    total_preds = []
    t0 = time.time()
    # iterate over batches
    for step, batch in tqdm(enumerate(val_dataloader)):

        # Progress update every 50 batches.
        if step % 50 == 0 and not step == 0:
            # Calculate elapsed time in minutes.
            elapsed = time.time() - t0

            # Report progress.
            print('  Batch {:>5,}  of  {:>5,}.'.format(step, len(val_dataloader)))

        # push the batch to gpu
        batch = [t.to(device) for t in batch]

        sent_id, mask, labels = batch

        # deactivate autograd
        with torch.no_grad():

            # model predictions
            preds = model(sent_id, mask)

            # compute the validation loss between actual and predicted values
            loss = cross_entropy(preds, labels)

            total_loss = total_loss + loss.item()

            # model predictions are stored on GPU. So, push it to CPU
            preds = preds.detach().cpu().numpy()
            labels_acc = labels.detach().cpu().numpy()

            y_preds.append(preds)
            y_trues.append(labels_acc)

            # print("Accuracy:", accuracy_score(labels_acc, np.argmax(preds, axis = 1)))
            acc_score = acc_score + accuracy_score(labels_acc, np.argmax(preds, axis = 1))
            total_preds.append(preds)

    # compute the validation loss of the epoch
    avg_loss = total_loss / len(val_dataloader)
    acc_score = acc_score / len(val_dataloader)
    # reshape the predictions in form of (number of samples, no. of classes)
    total_preds = np.concatenate(total_preds, axis = 0)

    y_preds=np.concatenate(y_preds,0)
    y_preds=np.argmax(y_preds, axis = 1)
    y_trues=np.concatenate(y_trues,0)

    recall=recall_score(y_trues, y_preds)
    precision=precision_score(y_trues, y_preds)   
    f1=f1_score(y_trues, y_preds)        
    accuracy=accuracy_score(y_trues, y_preds)

    result = {
        "eval_recall": float(recall),
        "eval_precision": float(precision),
        "eval_f1": float(f1),
        "eval_acc": float(accuracy),
    }

    print("***** Eval results *****")
    for key in sorted(result.keys()):
        print(f'{key}: {result[key]:.4f}')
    
    print(confusion_matrix(y_trues, y_preds))

    return avg_loss, acc_score