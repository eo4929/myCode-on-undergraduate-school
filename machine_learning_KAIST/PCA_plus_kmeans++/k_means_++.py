
import elice_utils
import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import KMeans
from random import *

def sample(X, probs):
    # Input : X = data points -> np.array
    # Input : probs = probability over data point X -> np.array
    # Return : sampled_center -> np.array
    #print(X[i])
    #i = np.random.choice(np.arange(X.shape[0]),p=probs)
    #return X[i]
    print(X)
    print(X.shape[0])
    #return np.random.choice(X.shape[0],1,p=probs)
    return X[np.random.choice(X.shape[0],1,p=probs)]

#def dist(row):
#    norm = lambda cent: np.linalg.norm(row - )

'''
def compute_probs(X,centers):
    def dist(row):
        norm = lambda cent: np.linalg.norm(row - cent, ord=2)
        return np.power(np.apply_along_axis(norm, axis=1, arr = centers).min(),2)
    dist_X = np.apply_along_axis(dist, axis=1, arr = X)
    
    return np.divide(dist_X, np.sum(dist_X))

'''

def compute_probs(X, centers): # 여기에 문제있나..?
    # Input : X = data points -> np.array
    # Input : centers = currently chosen centers -> np.array
    # Return : Probability distribution over data point -> np.array
    
    x = X.shape[0]
    
    data = np.zeros(X.shape[0])
    probs = np.zeros(X.shape[0])
    for i in range(X.shape[0]):
        data[i] = 9999999999999
        for j in range(centers.shape[0]):
            data2 = 0
            for k in range(X.shape[1]):
                #print(X[i][k])
                #print(centers[j][k])
                data2 += (X[i][k] - centers[j][k])**2
            if data2 < data[i]:
                data[i] = data2
    
    return data / np.sum(data)
    
def initialize(X, n_cluster):
    # Input : X -> np.array
    # Input : n_cluster = number of clusters -> int
    # Return : centers -> np.array
    #print(X) # n by 2
    #cen = np.zeros((0,X.shape[1]))
    
    probs = np.ones(len(X)) / len(X)
    centers = [sample(X,probs)]
    #centers = np.zeros((0,X.shape[1]))
    #print(X)
    for i in range(n_cluster-1):
        probs = compute_probs(X, np.array(centers))
        centers.append( [sample(X,probs)] ) 
    #while centers.shape[0] < n_cluster:
    #for _ in range(n_cluster-1):
        #centers = np.concatenate( [centers, sample(X,probs) ] )
        #probs = compute_probs(X,centers)
        #probs = compute_probs(X, np.array(centers))
        #centers.append( sample(X,probs) )
        #cen = np.concatenate( [cen, sample(X,probs) ] )
        
    #return np.array(cen)
    #return centers
    return np.array(centers)


def main():
	#### Generate data ####

    # Set mean and standard deviation of nomral distribution for each label
    mu = np.array([[1, 2], [1, -3], [5, 0]])
    std = 0.7

    # Set the number of samples
    Num = 50

    # Sample from the normal distributions
    X = np.concatenate([np.random.normal(loc = mu, scale = std) for i in range(Num)])
    y = np.concatenate([[0, 1, 2] for i in range(Num)])

    labels = set(y)

    colours = ['red','green','blue']

    # Initialize K-means center points
    centers = initialize(X, 3) # 여기서 한번 호출함

    preds = y

    #### Plot results of K-means clustering ####

    for i in range(10):
        # Create an empty pyplot figure
        plt.figure()

        # Scatter plot for each label
        for label in labels:
            idx = (preds == label)
            plt.scatter(X[idx, 0], X[idx, 1], color = colours[label], marker = '.')

        # Plot K-means center points
        plt.scatter(centers[:, 0], centers[:, 1], marker = 'x', color = 'black')

        # Set title
        plt.title('Iteration : ' + str(i))

        # Show the plot
        filename = 'kmeans' + str(i) + '.png'
        plt.savefig(filename)
        elice_utils.send_image(filename)

        # Proceed the next iteration of K-means clustering
        kmeans = KMeans(n_clusters = 3, n_init = 1, init = centers, max_iter = 1, random_state = 0).fit(X)
        preds = kmeans.labels_
        centers = kmeans.cluster_centers_

if __name__ == "__main__":
    main()

