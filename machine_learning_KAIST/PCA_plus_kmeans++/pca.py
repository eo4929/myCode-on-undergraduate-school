import matplotlib
matplotlib.use('Agg')

import sys
import numpy as np
import pylab as plt
import matplotlib.cm as cm
import elice_utils
import operator

def sample_data(n_samples):
    X = np.loadtxt("data/mnist2500_X.txt");
    labels = np.loadtxt("data/mnist2500_labels.txt");

    #sampling
    np.random.seed(0)
    sample_idx = np.random.choice(list(range(2500)), n_samples, replace=False)
    sampled_labels = labels[sample_idx]
    sampled_X = X[sample_idx]
    return sampled_X, sampled_labels


def plotting(Y, labels):
    # plot the results
    legend_ = []; colors = cm.rainbow(np.linspace(0, 1, 10))
    for i in sorted(list(set(labels))):
        idxs = (labels==i).nonzero()
        l = plt.scatter(np.squeeze(Y[idxs,0]), Y[idxs,1], 20, color=colors[int(i)])
        legend_.append(l)
    plt.legend(legend_, list(range(10)), loc='center left', ncol=1, fontsize=8, bbox_to_anchor=(1, 0.5))
    plt.savefig("result.png");
    elice_utils.send_image("result.png")
    return


def pca(X, no_dims):
    # implement pca function here.
    # Don't use scikit-learn, please use numpy only.
    # Input : X = data points (n, d) -> np.array
    # Input : no_dims = number of dimensions that we will project  -> int
    # Return : projected_X = Projected X on no_dim dimensions -> np.array
    # Add 1e-10 to avoid division-by-zero
    # STORE centered X on scaled_X
    
    scaled_X = None
    #print(X)
    projected_X = None
    ep = [1e-10] * 784
    #std_arr = np.add(np.std(X,axis=0),1e-10 ) 
    #print(std_arr)
    std_arr = np.std(X,axis=0)
    mean_arr = np.mean(X,axis=0)
    #print(len(mean_arr))
    
    Z = (X - mean_arr) / (std_arr + ep)
    scaled_X = Z
    #print(s)
    #Z = (X - (0,X[1])) / (1 + ep)
    u, s, vh = np.linalg.svd(scaled_X,full_matrices=True)
    projected_X = np.dot(X, vh[:no_dims].transpose())
    v = np.transpose(vh)
    sh = np.transpose(s)
    #print(len(sh))
    #print(s)
    #print(np.diag(s, 1)[:, 1:])
    #eigenVal = s * s
    #XXT = v * eigenVal * vh
    
    ev = []
    for i in range(no_dims):
        ev.append( vh[i] )
    ev = np.transpose(ev)
    #print(ev)
    #print(XXT)
    #ew, ev = np.linalg.eig(XXT)
    #print(len(ev[0]))
    #print(ev[0]) # 이게 맥스겠지?
    #print(np.asmatrix(s))
    #eigenVal = np.asmatrix(s) * np.asmatrix(sh)
    #print(len(eigenVal))
    proj_of_u_on_v = []
    for i in range(len(scaled_X)):
        proj_of_u_on_v.append( (np.dot(X[i] , ev) / ev**2) * ev )
        #proj_of_u_on_v.append( (np.dot(scaled_X[i] , ev) / ev**2) * ev )
    #print(np.array(proj_of_u_on_v))
    #print(u)
    #print(len(proj_of_u_on_v))
    
    return scaled_X, projected_X

def main():
    # load data
    X, labels = sample_data(1000) # x = 1000 * 784
    #print(len(X))
    #print(labels)

    # run pca
    Y = pca(X, 2)[1]

    # plotting
    plotting(Y, labels)

    return sys.modules


if __name__ == '__main__':
    main()

