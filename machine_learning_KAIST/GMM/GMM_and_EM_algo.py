from elice_utils import EliceUtils
from scipy.stats import multivariate_normal
from itertools import permutations
import numpy as np
import matplotlib.pyplot as plt
import test_variables

elice_utils = EliceUtils()

def generate_samples():
    num_samples = np.random.randint(2, 5)
    params = []
    sample_list = []
    
    while len(params) < num_samples:
        # Random mu
        new_mu = 20 * np.random.random((2,)) - 10
        separated = True
        
        for i in range(len(params)):
            if np.linalg.norm(np.subtract(params[i][0], new_mu)) < 3:
                separated = False
                break

        if not separated:
            continue
            
        #Random cov
        A = 2 * np.random.rand(2, 2) - 1
        
        params.append((new_mu, np.matmul(np.transpose(A), A)))
    
    params.sort(key=lambda e: e[0][0])
    ans = []
    for i in range(len(params)):
        num_points = np.random.randint(120, 140)
        sample_list.append(np.random.multivariate_normal(params[i][0], params[i][1], num_points))
        ans.extend([i] * num_points)
    
    samples = np.concatenate(sample_list)
    return samples, params, ans
    
def plot_samples(samples, color):
    def deco_plot(ax):
        ax.set_xlabel("X1", fontsize=15); ax.set_ylabel("X2", fontsize=15)
        ax.set_ylim([-15,15]); ax.set_xlim([-15,15])

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    for i in range(1):
        ax.scatter(samples[:,0], samples[:,1], c=color, s=20, alpha=.7, linewidth=0)
    deco_plot(ax)
    plt.savefig('samples.png')
    elice_utils.send_image('samples.png')
    plt.close()
        
# input -- k: number of clusters, data_cov: covariance of the given data
# output -- parameters: a list of length k, containing tuples (pi, mu, cov) 
# Please make sure that the mu and cov are np matrices with shapes (2, ) and (2, 2) respectively.
def initialize_parameters(k, data_cov):
    parameters = []
    pi = 1/k
    cov = data_cov
    for i in range(k):
        # Generate the parameters and return them
        mu = 20 * np.random.random((2,)) - 10
        parameters.append((pi, mu, cov))
        
    return parameters
    
# input -- samples: list of data points, param: current list of parameters
# output -- responsibility: array of size (n, k), containing responsibilities gamma_{nk},
#           where n is the number of data points
#
# Calculates the responsibility values for the data points
def expectation(samples, param):
    # x = np.linspace(0, 5, 10, endpoint=False)
    # print(x)
    N = len(samples)
    K = len(param)
    # responsibility = []
    responsibility = np.zeros((N, K))
    pi = np.array(param)[:, 0]

    # print(samples)
    # multivariate_normal.pdf(allow_singular=True)
    # multivariate_normal.pdf(samples[n], mu, cov, True)

    def helper(sample):
        def prob(param):  # param=(pi, mu, cov)
            return param[0] * multivariate_normal.pdf(sample, param[1], param[2], True)

        pi_N = np.apply_along_axis(prob, 1, np.array(param))
        gamma = np.divide(pi_N, pi_N.sum())
        return gamma

    multivariate_normal_pdf = []

    for k in range(K):
        mu = param[k][1]
        cov = param[k][2]
        multivariate_normal_pdf.append(multivariate_normal.pdf(samples, mu, cov, True))

    np_multivariate_normal_pdf = np.array(multivariate_normal_pdf)
    trans_multivariate_normal_pdf = np.transpose(np_multivariate_normal_pdf)

    for n in range(N):
        sum_multivariate_normal_pdf = 0

        for k in range(K):
            sum_multivariate_normal_pdf += pi[k] * trans_multivariate_normal_pdf[n][k]
        for k in range(K):
            responsibility[n][k] = pi[k] * trans_multivariate_normal_pdf[n][k] / sum_multivariate_normal_pdf
    '''
    sum1 = None
    for k in range(K):
        pi, mu, cov = param[k]
        sum1 += pi * multivariate_normal.pdf(samples[n], mu, cov, True)

    res1 = None
    responsibility2 = []

    for n in range(N):
        for k in range(K):
            above = pi * multivariate_normal.pdf(samples[n], mu, cov, True)
            responsibility2.append( above / sum1 )
        responsibility.append(responsibility2)
    '''

    # responsibility = np.apply_along_axis(helper, 1, np.array(samples))

    return responsibility.tolist()
    # return np.array(responsibility)


# input -- responsibility: (n, k) list of responsibilities, samples: list of data points, param: current list of parameters
# output -- parameters: new calculated list of parameters
#
# Updates the parameters pi, mu, cov from the responsibility values
def maximization(responsibility, samples, params):  # param=(pi, mu, cov)
    parameters = []
    # print(samples)
    # print(responsibility)
    # print(params)

    N = len(samples)
    K = len(params)
    # print(K)

    # print( np.sum(np.array(responsibility)) )
    # np_responsibility = np.array(responsibility)
    total_res = np.sum(responsibility, axis=0)

    # print(total_res)
    # new_pi = np.divide(total_res, np_responsibility.shape[0])
    # print(new_pi)
    new_pi = total_res / N

    # old_mu = []
    # for i in range(K): # 수정 필요
    #    old_mu.append( params[i][1] )
    # print(old_mu)

    old_mu = np.array(params)[:, 1]
    new_mu = []
    for k in range(K):
        data_sum = np.zeros(samples.shape[1])
        for n in range(N):
            data_sum += responsibility[n][k] * samples[n]
        new_mu.append(data_sum / total_res[k])
    new_mu = np.array(new_mu)
    new_cov = []
    sa = samples.shape[1]
    for k in range(K):
        data_sum = np.zeros((sa, sa))
        for n in range(N):
            sub_data_mu = np.array([samples[n] - old_mu[k]])
            data_sum += responsibility[n][k] * np.matmul(np.transpose(sub_data_mu), sub_data_mu)
        new_cov.append(data_sum / total_res[k])

    #new_mu = np.array(new_mu)
    new_cov = np.array(new_cov)
    #print(new_cov)
    # i=0
    # while i < K:
    #    parameters.append( (new_pi[i], new_mu[i], new_cov[i]) )
    #    i+=1
    for k in range(K):
        parameters.append((new_pi[k], new_mu[k], new_cov[k]))

    return parameters

# input -- samples: list of data points, param: current list of parameters
# output -- likelihood: the calculated log likelihood
#
# Calculates the log likelihood of the current classification
def log_likelihood(samples, param):
    likelihood = 0
    
    N = len(samples)
    K = len(param)
    
    for n in range(N):
        partial_sum = 0
        for k in range(K):
            pi, mu, cov = param[k]
            partial_sum += pi * multivariate_normal.pdf(samples[n], mu, cov, True)
        likelihood += np.log(partial_sum)
    
    return likelihood
    
# input -- samples: list of data points, param: current list of parameters
# output -- labels: the list of calculated labels for the samples, size = n
#
# Classifies the given data points into the clusters
def classify(samples, param):
    labels = []
    N = len(samples)
    K = len(param)
    multivariate_normal_pdf = []
    responsibility = np.zeros((N, K))  # 맞겠지? 틀리면 체크하기
    pi = np.array(param)[:, 0]

    for k in range(K):
        mu = param[k][1]
        cov = param[k][2]
        multivariate_normal_pdf.append(multivariate_normal.pdf(samples, mu, cov, True))

    np_multivariate_normal_pdf = np.array(multivariate_normal_pdf)
    trans_multivariate_normal_pdf = np.transpose(np_multivariate_normal_pdf)

    for n in range(N):
        sum_multivariate_normal_pdf = 0

        for k in range(K):
            sum_multivariate_normal_pdf += pi[k] * trans_multivariate_normal_pdf[n][k]
        for k in range(K):
            responsibility[n][k] = pi[k] * trans_multivariate_normal_pdf[n][k] / sum_multivariate_normal_pdf

    labels = np.argmax(responsibility, axis=1)
    labels = labels.tolist()
    return labels
    
# input -- samples: list of data points
# output -- labels: the list of calculated labels for the samples, size = n 
def train_and_label(samples):
    samplesT = samples.T
    data_cov = np.cov(samplesT)
    
    likelihood_list = []
    param_list = []
    
    for k in range(2,5):
        params = initialize_parameters(k, data_cov)
        
        likelihood = []
        for i in range(100):
            # E-step
            responsibility = expectation(samples, params)
            # M-step
            
            params = maximization(responsibility, samples, params)
            likelihood.append(log_likelihood(samples, params))
            
            if len(likelihood) > 1 and abs(likelihood[-1] - likelihood[-2]) < 1e-15:
                break
        
        likelihood_list.append(likelihood[-1])
        param_list.append(sorted(params, key=lambda e: e[1][0]))
    
    labels = classify(samples, param_list[np.argmax(likelihood_list)])
    
    return labels
    
def main():
    # Testing your expectation function
    test_samples = np.array([[0,1], [2,3], [4,5]])
    test_params = [(0.6, np.array([0, 1]), np.array([[1, 0], [0, 1]])), (0.4, np.array([2, 3]), np.array([[2, 1], [1, 2]]))]
    responsibility = expectation(test_samples, test_params)    
    try:
        assert(np.all(np.abs(np.array(responsibility) - np.array(test_variables.expected_responsibility)) < 1e-8))
    except:
        print("Expectation function is not implemented correctly.")
        return
    
    # Testing your maximization function
    try:
        parameters = maximization(test_variables.expected_responsibility, test_samples, test_params)
        a = [list(zip(parameters[i], test_variables.expected_parameters[i])) for i in range(2)]
        b = []
        for i in range(2):
            b.extend(map(lambda x: np.all(np.abs(x[0]-x[1]) < 1e-8), a[i]))

        assert(np.all(b))
    except:
        print("Maximization function is not implemented correctly.")
        return
    
    # Testing your classify function
    test_samples = np.array([[0, 1], [1.5, 2.5], [1, 2], [4, 5]])
    labels = classify(test_samples, test_variables.expected_parameters)
    try:
        assert(labels == [0, 1, 1, 1])
    except:
        print("Classify function is not implemented correctly.")
        return
        
    # You can change the seed to test with different cases
    np.random.seed(570)
    
    # generates data points in random gaussian distributions
    samples, prm, ans = generate_samples()
    # print(prm[0])
    # plots the generated samples
    plot_samples(samples, ans)
    
    # Classify the samples
    labels = train_and_label(samples)
    
    # Calculate the accuracy of the model
    max_correct = 0
    
    num_clusters = len(np.unique(ans))
    num_labels = len(np.unique(labels))
    
    if num_labels < num_clusters:
        perm = permutations(list(range(num_clusters)), num_labels)
        for x in perm:
            curr_correct = 0
            for i in range(len(ans)):
                if ans[i] == x[labels[i]]:
                    curr_correct += 1
            max_correct = max(curr_correct, max_correct)
    else:
        perm = permutations(list(range(num_labels)), num_clusters)
        
        for x in perm:
            curr_correct = 0
            for i in range(len(ans)):
                if labels[i] == x[ans[i]]:
                    curr_correct += 1
            max_correct = max(curr_correct, max_correct)
    
    print("Accuracy: %.2f%%" % (max_correct/len(ans)*100))
    print("Original number of clusters: %d" % num_clusters)
    print("Calculated number of clusters: %d" % num_labels)

if __name__ == "__main__":
    main()
