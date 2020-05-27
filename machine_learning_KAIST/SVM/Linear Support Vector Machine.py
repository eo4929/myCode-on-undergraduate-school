
import numpy as np
import scipy.optimize
import math
import draw_plot as dp
import generate_data as gd

def F(W_b):
    # To-do: Define your target function to minimize
    w1 = W_b[0]
    w2 = W_b[1]
    b = W_b[2]

    return ( (W_b[0] * W_b[0]) + ( W_b[1] * W_b[1] ) ) / 2

def SVM(X, Y):
    #print(X)
    #print(Y)
    '''
    # In function SVM, we have to find the optimized W and b from scipy solver
    # The solver is already defined
    # We need to define constraint of the solver
    # From the given constraint as in description, set the appropriate type 'cons_type' and fuction 'cons_func'
    # For constraints, the given constraints should be transformed in the form of g(x) >= 0
    # Based on transformed constraints, define 'cons_type' and fuction 'cons_func'
    '''

    # To-do: Define transformed constraint function g(x)
    def cons_func(W_b):
        w1 = W_b[0]
        w2 = W_b[1]
        b = W_b[2]
        res = []
        for i in range(len(Y)):
                res.append(( Y[i] * ( (X[i][0] * w1) + (X[i][1] * w2) + b) ) -1)
        return res

    # To-do: Define constraint type
    cons_type = 'ineq'


    ########### Do not Modify ################
    #cons = {'type': cons_type ,'fun': cons_func} # constraints
    cons = ({'type': cons_type,
          'fun' :cons_func})
    W_b = np.random.randn(3) # Initialize W_b
    res_cons = scipy.optimize.minimize(F, W_b, constraints=cons, method='SLSQP')
    W_b = res_cons['x']
    W, b = W_b[0:2], W_b[2]
    ##########################################
    return W, b

def get_max_margin(W):
    # To-do: Define maximum margin
    W = np.array(W)
    return 1 / np.linalg.norm(W)

def find_support_vectors(X, Y, W, b, epsilon=1e-5):
    '''
    # Find support vectors and print the indices of them
    # Index starts from 0
    # Use epsilon to give some margin when checking if a vector meets hyperplane
    '''
    w1 = W[0]
    w2 = W[1]
    # ToDo:Get support_vectors_idx
    support_vectors_idx = []
    
    for i in range(len(Y)):
        # if (( (X[i][0] * w1) + (X[i][1] * w2) + b) -1 <= epsilon) and (( (X[i][0] * w1) + (X[i][1] * w2) + b) +1 >= -epsilon):
        # if (( (X[i][0] * w1) + (X[i][1] * w2) + b) -1 <= epsilon) and (( (X[i][0] * w1) + (X[i][1] * w2) + b) +1 >= -epsilon):
            if (abs( (Y[i] * ((X[i][0] * w1) + (X[i][1] * w2) + b)) - 1) <= epsilon):
                support_vectors_idx.append(i)

    return support_vectors_idx



def main():
    X, Y = gd.get_dataset(100)
    W, b = SVM(X, Y)
    max_margin = get_max_margin(W)
    support_vectors_idx = find_support_vectors(X, Y, W, b)
    print("support_vectors_idx: ", support_vectors_idx)

    # This function plot randomly generated datapoints from function get_dataset
    dp.draw_datasetonly(X, Y)

    # Comment out the function below after you optimize all the parameters and find the support vectors!
    # dp.draw(X, Y, W, b, support_vectors_idx)


if __name__ == "__main__":
    main()