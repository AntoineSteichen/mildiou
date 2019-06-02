#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

humidity_training_complete = pd.read_csv('data/humidity-2018.csv')
humidity_training_processed = humidity_training_complete.iloc[:, 4:5].values

from sklearn.preprocessing import MinMaxScaler

scaler = MinMaxScaler(feature_range = (0, 1))

humidity_training_scaled = scaler.fit_transform(humidity_training_processed)

X_train = []
y_train = []
for i in range(60, 1258):
    X_train.append(humidity_training_scaled[i-60:i, 0])
    y_train.append(humidity_training_scaled[i, 0])
X_train, y_train = np.array(X_train), np.array(y_train)


X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1))

from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Dropout

regressor = Sequential()

regressor.add(LSTM(units = 50, return_sequences = True, input_shape = (X_train.shape[1], 1)))
regressor.add(Dropout(0.2))

regressor.add(LSTM(units = 50, return_sequences = True))
regressor.add(Dropout(0.2))

regressor.add(LSTM(units = 50, return_sequences = True))
regressor.add(Dropout(0.2))

regressor.add(LSTM(units = 50))
regressor.add(Dropout(0.2))

regressor.add(Dense(units = 1))

regressor.compile(optimizer = 'adam', loss = 'mean_squared_error')

regressor.fit(X_train, y_train, epochs = 200, batch_size = 256)

dataset_test = pd.read_csv('data/H.csv')
real_humidity = dataset_test.iloc[:, 4:5].values

dataset_total = pd.concat((humidity_training_complete['Humidity'], dataset_test['Humidity']), axis = 0)
inputs = dataset_total[len(dataset_total) - len(dataset_test) - 60:].values
inputs = inputs.reshape(-1,1)
inputs = scaler.transform(inputs)
X_test = []
for i in range(60, 3000):
    X_test.append(inputs[i-60:i, 0])
X_test = np.array(X_test)
X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))
predicted_Humidity = regressor.predict(X_test)
predicted_Humidity = scaler.inverse_transform(predicted_Humidity)
np.savetxt('predictedHumidity.csv',predicted_Humidity,fmt='%.2f', delimiter=',')

plt.plot(real_humidity, color = 'red', label = 'Real Humidity')
plt.plot(predicted_Humidity, color = 'blue', label = 'Predicted Humidity')
plt.title('Humidity Prediction')
plt.xlabel('Time')
plt.ylabel('Humidity')
plt.legend()
plt.axis([0,1200,50,110])
plt.savefig('humidity.png')
plt.show()

