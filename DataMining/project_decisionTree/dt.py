from sklearn import tree
from sklearn import preprocessing
from sklearn.metrics import classification_report,confusion_matrix
import sys
import numpy as np
#dTree = tree.DecisionTreeClassifier(criterion="gini") # use gini
dTree = tree.DecisionTreeClassifier(criterion="entropy") # use information gain(entropy) -> 이게 낫다
attri_name_1_N = ''

def getTrainingInputFile(inputfile,count_training_set,training_label,training_attri_from1_toN_1):
    global attri_name_1_N

    with open(inputfile, 'r') as f:
        tuples_training = f.read().split('\n')
        #print('tuples_training: ',end='')
        #print(tuples_training)

        label = ''
        count = 0 # 맨 첫줄은 버리기 위함
        for attributes_from1_toN in tuples_training:
            if count == 0:
                attri_name_1_N = attributes_from1_toN
                count+=1
                continue
            else:
                count_training_set += 1

            attributes_from1_toN = attributes_from1_toN.split('\t')
            
            label = attributes_from1_toN.pop()
            training_attri_from1_toN_1.append(attributes_from1_toN)
            training_label.append(label)

            #print('attributes_from1_toN: ' + str(count_training_set),end='')
            #print(attributes_from1_toN)
        training_attri_from1_toN_1.pop() #마지막 empty ele 제거
        training_label.pop() #마지막 empty ele 제거
        #for val in training_label:
        #    print('training_label: ',end='')
        #    print(val)
        #for val in training_attri_from1_toN_1:
        #    print('training_attri_from1_toN_1: ',end='')
         #   print(val)

def getTestInputFile(inputfile, count_test_set, test_attri):

    with open(inputfile, 'r') as f:
        tuples_test = f.read().split('\n')
        count = 0 # 맨 첫줄은 버리기 위함
        for tuple_test in tuples_test:
            if count == 0:
                count+=1
                continue
            else:
                count_test_set += 1 # 디버깅용
            
            tuple_test = tuple_test.split('\t')
            test_attri.append(tuple_test)
        
        test_attri.pop()
        #count=1
        #for row in test_attri:
        #    print(str(count)+' ' ,end='')
        #    print(row)
        #    count+=1

def makeResultList(test_attri,prediction_each_row):
    prediction_each_row = np.array(prediction_each_row).tolist()
    prediction_each_row.reverse()

    oneline = ''
    oneline = attri_name_1_N + '\n'
    f.write(oneline)
    oneline = ''

    for row in test_attri:
        row.append(prediction_each_row.pop())
        #print(row)

        for ele in row:
            oneline = oneline + ele + '\t'
        oneline = oneline[0:len(oneline)-1] # 마지막 \t는 제거
        oneline = oneline + '\n'
        #print(oneline)
        f.write(oneline)
        oneline = ''


if __name__ == '__main__':
    argv = sys.argv

    training_set_file = argv[1]
    test_set_file = argv[2]
    result_set_file = argv[3]

    count_training_set = 0
    training_attri_from1_toN_1 = list()
    training_label = list()

    count_test_set = 0
    test_attri = list()

    getTrainingInputFile(training_set_file,count_training_set,training_label,training_attri_from1_toN_1)
    getTestInputFile(test_set_file,count_test_set,test_attri)

    encode = preprocessing.OneHotEncoder()
    encode_training_attri_from1_toN_1 = encode.fit_transform(training_attri_from1_toN_1).toarray()

    dTree = dTree.fit(encode_training_attri_from1_toN_1,training_label)

    encode_test_attri = encode.transform(test_attri).toarray()
    prediction_each_row = dTree.predict(encode_test_attri) # 한줄씩 넣지 말고 한꺼번에 넣자 encode.fit_transform가 파라미터로 [[]] 이걸 받는다
    #print(prediction_each_row)

    accuracy = dTree.score( encode_test_attri , prediction_each_row )
    #print("accuracy: " + str(accuracy))

    f = open(result_set_file,'w')
    makeResultList(test_attri , prediction_each_row)
    f.close()
    