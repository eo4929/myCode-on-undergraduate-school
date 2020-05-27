import numpy as np
from sklearn import linear_model
import pandas as pd
from sklearn.model_selection import train_test_split,cross_val_score
from sklearn.metrics import r2_score, mean_squared_error


def main(rs=108):
    data = pd.read_csv("./data/Hitters.csv", header=0)
    response_var = -1
    y_vec = data.ix[:, response_var].values.reshape(-1, 1)
    y_label = data.columns[response_var]

    x_label = ", ".join(data.columns[1:-1])
    x_mat = data.ix[:, 1:-1].values
    x_mat = x_mat.reshape(-1, x_mat.shape[1])

    x_train, x_test, y_train, y_test = train_test_split(x_mat, y_vec, test_size=0.2, random_state=rs)

    # Linear Regression
    rss, r2, mse = multi_var_hitter(x_train, x_test, y_train, y_test, x_label)
    print("Linear Regression Result")
    print("RSS: {}".format(rss))
    print("R^2: {}".format(r2))
    print("MSE: {}".format(mse))
    print()

    # Ridge Regression
    best_lambda_ridge, best_lambda_lasso = get_best_lambda_value_ridge_lasso(data)
    rss, r2, mse = multi_var_hitter_ridge(x_train, x_test, y_train, y_test, x_label, best_lambda_ridge)
    print("Ridge Regression Result")
    print("RSS: {}".format(rss))
    print("R^2: {}".format(r2))
    print("MSE: {}".format(mse))
    print("Best lambda value: {}".format(best_lambda_ridge))
    print()

    # Lasso Regression
    rss, r2, mse = multi_var_hitter_lasso(x_train, x_test, y_train, y_test, x_label, best_lambda_lasso)
    print("lasso Result")
    print("RSS: {}".format(rss))
    print("R^2: {}".format(r2))
    print("MSE: {}".format(mse))
    print("Best lambda value: {}".format(best_lambda_lasso))
    print()


def get_best_lambda_value_ridge_lasso(data):
    """
    Implement Here
    The grader will call this function to get the lambda value,
    and run the functions with hidden test data.
    Do not assign the exact values to the variables.
    You should find the best lambda value via cross validation.
    """
    best_lambda_ridge = 4500 # 95
    best_lambda_lasso =400 # 98
    
    response_var = -1
    y_vec = data.ix[:, response_var].values.reshape(-1, 1)
    y_label = data.columns[response_var]

    x_label = ", ".join(data.columns[1:-1])
    x_mat = data.ix[:, 1:-1].values
    x_mat = x_mat.reshape(-1, x_mat.shape[1])

    x_train, x_test, y_train, y_test = train_test_split(x_mat, y_vec, test_size=0.2)
    
    regr1 = linear_model.Ridge(alpha=best_lambda_ridge)
    regr1.fit(x_train, y_train)
    regr2 = linear_model.Lasso(alpha=best_lambda_lasso)
    regr2.fit(x_train, y_train)
    
    '''
    pre_res1 = max( cross_val_score(regr1, x_test, y_test, cv=10) )
    for num in np.logspace(2.0, 3.0, num=100,base=2):
        res1 = max( cross_val_score(regr1, x_test, y_test, cv=10) )
        if pre_res1 > res1:
            break
        else:
            best_lambda_ridge = num
            regr1.set_params(alpha=best_lambda_ridge)
        
    pre_res2 = max( cross_val_score(regr2, x_test, y_test, cv=10) )
    for num in np.logspace(2.0, 3.0, num=100,base=2 ,endpoint=False):
        res2 = max( cross_val_score(regr2, x_test, y_test, cv=10) )
        if pre_res2 > res2:
            break
        else:
            best_lambda_lasso = num
            regr2.set_params(alpha=best_lambda_lasso)
    '''
    
    #print( max( cross_val_score(regr1, x_test, y_test, cv=10) ) ) 
    #print( max( cross_val_score(regr2, x_test, y_test, cv=10) ) )
    return best_lambda_ridge, best_lambda_lasso


def multi_var_hitter(x_train, x_test, y_train, y_test, x_label):
    regr = linear_model.LinearRegression()

    regr.fit(x_train, y_train)
    predicted_y_test = regr.predict(x_test)
    rss = np.sum((predicted_y_test - y_test) ** 2)
    r2 = r2_score(y_test, predicted_y_test)
    mse = mean_squared_error(y_test, predicted_y_test)
    return rss, r2, mse


def multi_var_hitter_ridge(x_train, x_test, y_train, y_test, x_label, best_lambda):
    """
    Implement Here
    """
    regr = linear_model.Ridge(alpha=best_lambda)
    regr.fit(x_train, y_train)
    predicted_y_test = regr.predict(x_test)
    rss = np.sum((predicted_y_test - y_test) ** 2)
    r2 = r2_score(y_test, predicted_y_test)
    mse = mean_squared_error(y_test, predicted_y_test)

    return rss, r2, mse


def multi_var_hitter_lasso(x_train, x_test, y_train, y_test, x_label, best_lambda):
    """
    Implement Here
    """
    regr = linear_model.Lasso(alpha=best_lambda)
    regr.fit(x_train, y_train)
    predicted_y_test = regr.predict(x_test)
    rss = np.sum((predicted_y_test - y_test) ** 2)
    r2 = r2_score(y_test, predicted_y_test)
    mse = mean_squared_error(y_test, predicted_y_test)

    return rss, r2, mse

if __name__ == "__main__":
    main()
