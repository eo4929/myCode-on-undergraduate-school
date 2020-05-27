from sklearn import datasets
from sklearn.linear_model import LogisticRegression
import numpy as np


def load_data():
    return datasets.load_iris(return_X_y=True)


def binary_classification(X, binary_label):
    # input : X (features), shape:(number of samples, number of features)
    # input : binary_label (1 or 0), shape:(number of samples, )
    # output : probability to belong to that class, shape:(number of samples, )
    binary_model = LogisticRegression(solver='lbfgs')  # Do not change the solver.
    # print(binary_label)
    binary_model.fit(X, binary_label)
    # print(binary_label)
    predicted_p = binary_model.predict_proba(X)
    # print(predicted_p)
    class1 = []
    for i in range(len(predicted_p)):
        # print(predicted_p[i][1])
        class1.append(predicted_p[i][1])
    # print(predicted_p)
    # predicted_p = sum(class1) / len(class1)
    predicted_p = class1
    predicted_p = np.array(predicted_p)
    predicted_p.transpose()
    print(np.array(predicted_p))
    #print(predicted_p.transpose())

    return np.array(predicted_p.transpose()) # here
    # return np.array(class1)


def multiclass_classification(X, y):
    # input : X (features), shape:(number of samples, number of features)
    # input : y (labels), shape:(number of samples,)
    # output : multiclass classification accuracy, shape:(1, )
    res = -1
    label = -1
    # for i in range(len(y)):
    #    p_tmp = binary_classification(X,i)
    #    if p_tmp>res:
    #        res = p_tmp
    #        label = i
    arg = []
    arg1 = binary_classification(X, y == 0)
    #arg.append(sum(arg1) / len(arg1))
    arg2 = binary_classification(X, y == 1)
    #arg.append(sum(arg2) / len(arg2))
    arg3 = binary_classification(X, y == 2)
    #arg.append(sum(arg3) / len(arg3))
    lab = -1
    for i in range(len(y)):
        acc = max(arg1[i],arg2[i],arg3[i])
        if acc == arg1[i]:
            lab = 0
        elif acc == arg2[i]:
            lab = 1
        elif acc == arg3[i]:
            lab = 2
        arg.append(lab)

    cnt = 0
    for i in range(len(y)):
        # print(y[i])
        if arg[i] == y[i]:
            cnt += 1
    # print(len(X[1]))
    # print(cnt)
    accuracy = cnt / len(y)

    return accuracy


def main():
    data = load_data()
    #acc = binary_classification(data[0], data[1] == 1)
    #print( sum(acc) / len(acc))
    #print(data[1])
    accuracy = multiclass_classification(data[0], data[1])
    print('Accuracy of multiclass classification is %f.' % (accuracy))


if __name__ == '__main__':
    main()