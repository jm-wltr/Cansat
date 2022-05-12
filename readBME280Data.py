import serial

cansatConn = serial.Serial("com6", "9600")

while True:
    if (cansatConn.inWaiting()>0):
        data = str(cansatConn.readline())
        print(str(data))

        aposPos = data.find("'")
        comaPos = data.find(",")
        temperature = data[aposPos + 1: comaPos]
