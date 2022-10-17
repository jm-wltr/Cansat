import serial

bme280Data = serial.Serial('com4', 9600)

while True:
    if(bme280Data.inWaiting()>0):
        myData = bme280Data.readline()
        print(myData)
