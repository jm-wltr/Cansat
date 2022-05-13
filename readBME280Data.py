import serial

cansatConn = serial.Serial("com6", "9600")



while True:
    if (cansatConn.inWaiting()>0):
        data = str(cansatConn.readline())
        print(str(data))

        #slicing strings
        aposPos = data.find("'")
        comaPos = data.find(",")
        colonPos = data.find(":")
        semiColonPos = data.find(";")
        hyphenPos = data.find("-")

        temperature = data[aposPos + 1: comaPos]
        pressure = data[comaPos + 2: colonPos]
        altitude = data[colonPos + 2: semiColonPos]
        humidity = data[semiColonPos + 2: hyphenPos]