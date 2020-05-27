from elice_utils import EliceUtils
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from math import sin

elice_utils = EliceUtils()

def sample(n):
    # Generating 'n' number of samples
    np.random.seed(2019)

    x = np.linspace(0, 5, n)  # Do not change this line.
    y = None
    y1 = [sin(1.5 * x1) for x1 in x]
    y2 = [2*ele for ele in y1 ]
    #reg =np.random.normal(0,1)
    y = np.array([ ele+ np.random.normal(0,1) for ele in y2  ])
    return x, y


def get_regression_model():
    # Generating Linear Regression model

    model = None
    model = LinearRegression()
    return model


def fit_regression_model(model, x, y, degree):
    # Fitting regression model

    fitted_model = None
    fitted_model = model.fit(np.vander(x, degree+1),y)
    #print(np.vander(x, degree+1))
    return fitted_model


def apply_polynomial(model, x):
    # Applying the model to input data
    # input : model (linear regression model), x (input sample)

    pred_y = None
    #print(np.vander(x, degree+1))
    pred_y = model.predict(np.vander(x, model.coef_.size))
    print(pred_y)
    return pred_y


def plot_figure(x, y, pred_y):
    # (Optional) Plotting the result of linear regression 
    plt.plot(x, y, 'k.')
    plt.plot(x, pred_y, 'r')

    plt.savefig('./linear_regression.png')
    #elice_utils.send_image('linear_regression.png')


def main():
    x, y = sample(50)

    model = get_regression_model()
    fitted_model = fit_regression_model(model, x, y, degree=5)
    print(y)
    pred_y = apply_polynomial(model, x)
    #print(pred_y)
    # Uncomment this line to draw the plot
    # plot_figure(x, y, pred_y)


if __name__ == "__main__":
    main()
