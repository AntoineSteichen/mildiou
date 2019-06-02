#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

temperature_training_complete = pd.read_csv('data/temperature-2018.csv')
temperature_training_processed = temperature_training_complete.iloc[:, 4:5].values

from sklearn.preprocessing import MinMaxScaler

scaler = MinMaxScaler(feature_range = (0, 1))

temperature_training_scaled = scaler.fit_transform(temperature_training_processed)

X_train = []
y_train = []
for i in range(60, 1258):
    X_train.append(temperature_training_scaled[i-60:i, 0])
    y_train.append(temperature_training_scaled[i, 0])
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

dataset_test = pd.read_csv('data/T.csv')
real_temperature = dataset_test.iloc[:, 4:5].values

dataset_total = pd.concat((temperature_training_complete['temperature'], dataset_test['temperature']), axis = 0)
inputs = dataset_total[len(dataset_total) - len(dataset_test) - 60:].values
inputs = inputs.reshape(-1,1)
inputs = scaler.transform(inputs)
X_test = []
for i in range(60, 3000):
    X_test.append(inputs[i-60:i, 0])
X_test = np.array(X_test)
X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))
predicted_temperature = regressor.predict(X_test)
predicted_temperature = scaler.inverse_transform(predicted_temperature)
np.savetxt('predictedTemperature.csv',predicted_temperature,fmt='%.2f', delimiter=',')

plt.plot(real_temperature, color = 'red', label = 'Real temperature')
plt.plot(predicted_temperature, color = 'blue', label = 'Predicted temperature')
plt.title('temperature Prediction')
plt.xlabel('Time')
plt.ylabel('temperature')
plt.legend()
plt.savefig('temperature.png')
plt.axis([0,1200,-10,50])
plt.show()

