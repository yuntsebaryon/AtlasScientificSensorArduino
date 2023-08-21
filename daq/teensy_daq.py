# %pip install pyserial
# 'pyserial' should be insstalled to import serial

from os.path import exists
import time
import serial
import pandas as pd
import csv

sensor = serial.Serial( '/dev/cu.usbmodem116218601', 9600 )
# sensor = serial.Serial( '/dev/cu.HC-05', 115200 )
# 'COM5' is the port name that the Arduino is connected.
# '9600' is the Baudrate.

fName = '/Users/yuntse/data/slarchetto/sensor_daq/sensor_20230820.csv'
# data = pd.DataFrame()
if not exists( fName ):
    f = open( fName, mode = 'w', newline = '' )
    CSVwriter = csv.DictWriter( f, ( 'Hour', 'Minute', 'Second', 'Humidity', 'Air_Temp', 'Dew_Temp', 'O2', 'TC_Temp' ))
    CSVwriter.writeheader()
    f.close()

while True:
    signal = sensor.readline()
    if not type(signal[0]) == int:
        continue

    print('sensor', time.strftime('%Y-%m-%d %H:%M:%S'), signal)
    value = signal.decode('utf-8').strip().split(' ')

    f = open( fName, 'a', newline = '' )
    wr = csv.writer(f)
    wr.writerow(value)
    f.close()

    time.sleep(1)
