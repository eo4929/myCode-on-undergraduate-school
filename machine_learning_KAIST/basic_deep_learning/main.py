from pandas import read_csv
from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from elice_utils import EliceUtils
import tensorflow.keras as keras

elice_utils = EliceUtils()

def preprocess(data_frame):
    # TODO: process the data into inputs (X) and output (y)
    #       Use one-hot encoding for the sex feature.
    #       You might want to normalize the input features.
    #       Please make sure that the returned values are array-like data.
    
    #X = data_frame.iloc[:,1].values
    #Y = data_frame.iloc[:,7].values
    #print(X)
    feature = data_frame.loc[0]
    #print(feature)
    sex = data_frame['Sex']
    #print(sex)
    
    min_max_scaler = preprocessing.MinMaxScaler()
    onehotencoder = preprocessing.OneHotEncoder()
    #min_max_scaler.fit(output)
    
    #print(data_frame['Sex'])
    sex = data_frame['Sex']
    #onehotencoder.fit(sex.values.reshape(-1,1))
    sex = onehotencoder.fit_transform(sex.values.reshape(-1,1)).toarray()
    #print(sex)
    
    #print(sex.tolist())
    #print(sex[0][0])
    X = []
    y = []
    #sex = []
    
    #for i, row in data_frame.iterrows():
    #    sex.append(row['Sex'])
    
    #sex = onehotencoder.transform(sex).toarray()
    
    for i, row in data_frame.iterrows():
        eachRow = []
        #print(row['Sex'])
        #sex.append(row['Sex'])
        y.append(row['Rings'])
        eachRow.append(sex[i][0])
        eachRow.append(sex[i][1])
        eachRow.append(sex[i][2])
        
        eachRow.append(row['Length'])
        eachRow.append(row['Diameter'])
        eachRow.append(row['Height'])
        eachRow.append(row['Whole weight'])
        eachRow.append(row['Shucked weight'])
        eachRow.append(row['Viscera weight'])
        eachRow.append(row['Shell weight'])
        
        X.append(eachRow)
    
    
    #print(X)
    X = preprocessing.scale(X)
    return (X, y)

def split_data(X, y):
    # TODO: Split the data into train and test datasets
    #       Use 1/4 of the data for testing
    #       Please make sure that the returned values are array-like data.
    
    X_train, X_test, y_train, y_test= train_test_split(X,y,test_size=0.25, random_state=1004)
    
    #X_train = None
    #X_test = None
    #y_train = None
    #y_test = None
    #print((X_train, X_test, y_train, y_test))
    return (X_train, X_test, y_train, y_test)
    
def model_builder(X_train, y_train): # 수정해야 함
    # TODO: Design a deep learning model
    # Experiment with the design and see which one performs the best.
    # You can change the loss, optimizer, add more layers, etc...
    
    #print(X_train)
    #print(y_train)
    
    model = keras.Sequential()
    
    model.add(keras.layers.Dense(64, activation='relu', input_shape=(10,)))
    model.add(keras.layers.Dense(64, activation='relu'))
    model.add(keras.layers.Dense(1, activation='relu'))
    
    model.compile(loss="mean_squared_error",
                  optimizer="adam",
                  metrics=['mse'])
                  
    model.fit(X_train, y_train, epochs=40, batch_size=10)
    
    return model
    
def predict(model, X): # 안건드려도 될듯
    # TODO: given the model, predict the values of 'Rings' given the input data and store them in the list
    
    predicted_age = model.predict(X)
    
    return predicted_age

def main():
    data_frame = read_csv('data/train.csv')
    #print(data_frame)
    
    X, y = preprocess(data_frame)
    
    X_train, X_test, y_train, y_test = split_data(X, y)
    
    model = model_builder(X_train, y_train)
    
    predicted_age = predict(model, X_test)
    
    mse = 0
    for i in range(len(predicted_age)):
        mse += (y_test[i] - predicted_age[i]) ** 2
    mse /= len(predicted_age)

    print('Mean Squared Error: %.3f' % mse)

if __name__ == "__main__":
    main()
