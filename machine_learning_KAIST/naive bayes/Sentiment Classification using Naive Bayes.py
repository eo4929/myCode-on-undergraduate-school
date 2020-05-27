import io
import numpy
import matplotlib as mpl
mpl.use("Agg")
import matplotlib.pyplot as plt
import statsmodels.api
import elice_utils_emotion
import re
import math
import os

def main():
    #a = create_BOW('hi my dae-young hi')
    #print(a[0])
    #print(a[1])

    pos_sentences = read_text_data('./txt_sentoken/pos/')
    neg_sentences = read_text_data('./txt_sentoken/neg/')
    testing_sentence = input("Input Sentence > ")

    ###############
    #Do not Change#
    ###############
    alpha = 0.1
    prob1 = 0.5 #Prior of POSITIVE
    prob2 = 0.5 #Prior of NEGATIVE

    prob_pair = naive_bayes(pos_sentences, neg_sentences, testing_sentence, alpha, prob1, prob2)

    print("P(pos|sentence) : ", prob_pair[0])
    print("P(neg|sentence) : ", prob_pair[1])

def naive_bayes(pos_sentence, neg_sentence, testing_sentence, alpha, prob1, prob2):
    '''
    By using Naive Bayes Classifier
    Calculate log P(class1|test), log P(class2|test)
    return normalized probability.

    prob1 == P(class1)
    prob2 == P(class2)
    Be careful prob1, prob2 are not log probabilities. # care
    
    But, to make assignment easy, it is already implemented.
    '''

    pos_model = create_BOW(pos_sentence)
    neg_model = create_BOW(neg_sentence)
    testing_model = create_BOW(testing_sentence)

    print(testing_model)

    pos_prob = calculate_doc_prob(pos_model, testing_model, alpha) + math.log(prob1)
    neg_prob = calculate_doc_prob(neg_model, testing_model, alpha) + math.log(prob2)
    
    print(pos_prob)
    print(neg_prob)

    return normalize_log_prob(pos_prob, neg_prob)


def read_text_data(directory):
    '''
    This function is already implemented.
    '''

    files = os.listdir(directory)
    files = [f for f in files if f.endswith('.txt')]

    all_text = ''
    for f in files:
        all_text += ' '.join(open(directory + f).readlines()) + ' '

    return all_text

def normalize_log_prob(logprob1, logprob2):
    '''
    logprob1, logprob2 are log probabilities.
    By normalizing these, return 2 probabilities that satisfy normalized_prob1 + normalized_prob2 = 1.
    You can transform log distribution to linear distribution by using math.exp().


    In this step, keep in mind if both log probabilities is very small, it can be 0 when it transformed to linear distribution.
    So, you have to do something before transforming.

    Example)
    logprob1 = -298
    logprob2 = -300
    In this case, it will be 0 when we use exp(). (underflow)
    So, we should add 298 to each probability.

    logprob1 = 0
    logprob2 = -2
    Then we apply exp(),

    prob1 = 1
    prob2 = 0.1353
    and normalizing these probabilities, we can finally get

    normalized_prob1 = 0.8808
    normalized_prob2 = 0.1192
    '''
    
    #Your code here
    maxprob = max(logprob1, logprob2)

    logprob1 -= maxprob
    logprob2 -= maxprob

    prob1 = math.exp(logprob1)
    prob2 = math.exp(logprob2)

    normalized_prob1 = prob1 / (prob1 + prob2)
    normalized_prob2 = prob2 / (prob1 + prob2)
    
    print(normalized_prob1 + normalized_prob2 )
    
    return (normalized_prob1, normalized_prob2)




def calculate_doc_prob(training_bow, testing_bow, alpha):
    logprob = 0

    train_dict = training_bow[0]
    train_bow = training_bow[1]
    test_dict = testing_bow[0]
    test_bow = testing_bow[1]

    sum_train = sum(training_bow[1])
    len_train = len(training_bow[0])

    for word in test_dict:
        word_freq = test_bow[ test_dict[word] ]
        word_freq_train = 0

        if word in train_dict:
            word_freq_train = train_bow[ train_dict[word] ]

        for i in range(0, word_freq):
            logprob += math.log(word_freq_train + alpha)
            logprob -= math.log(sum_train + len_train * alpha)

    return logprob



def create_BOW(sentence):

    '''
    function that makes Bag of Words.
    '''
    # Your code here
    
    sentenceNew = sentence.lower()
    sentenceNew = replace_non_alphabetic_chars_to_space(sentenceNew)
    tokens = sentenceNew.split(' ')

    bow_dict = {}
    bow = []
    for token in tokens:
        if len(token) < 1: continue
        if token not in bow_dict:
            new_idx = len(bow)
            bow.append(0)
            bow_dict[token] = new_idx
        bow[bow_dict[token]] += 1
    '''
    for voca in token:
        if voca == '':
            continue
        if voca not in word2index.keys():
            word2index[voca] = len(word2index)
            #word2index[voca] = len(bow)
            #bow.insert(len(word2index) - 1, 1)
            bow.append(1)
        else:
            index = word2index.get(voca)
            bow[index] = bow[index] + 1
    '''

    return bow_dict, bow

def replace_non_alphabetic_chars_to_space(sentence):
    return re.sub(r'[^a-z]+', ' ', sentence)


if __name__ == "__main__":
    main()
