import numpy as np

def linear_func1(x):
    l = len(x)
    return (3*x + 100 + 30 * np.random.randn(l))

def linear_func2(x):
    l = len(x)
    return (3*x - 100 + 30 * np.random.randn(l))
    
def get_dataset(n):
    np.random.seed(32840091)
    x1_1 = (np.random.random(int(0.5 * n)) - 0.5) * 100
    x2_1 = linear_func1(x1_1)
    x1_2 = (np.random.random(int(0.5 * n)) - 0.5) * 100
    x2_2 = linear_func2(x1_2)
    y_1 = np.ones(int(0.5 * n))
    y_2 = -1 * np.ones(int(0.5 * n))

    x1 = np.concatenate((x1_1, x1_2))
    x2 = np.concatenate((x2_1, x2_2))
    Y = np.concatenate((y_1, y_2))
    X = np.array(list(zip(x1, x2)))

    return (X, Y)