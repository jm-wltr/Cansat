import serial

cansatConn = serial.Serial("com4", "9600")



while True:
    if (cansatConn.inWaiting()>0):
        data = str(cansatConn.readline())
        print(data)
        #slicing strings
        aposPos = data.find("'")
        comaPos = data.find(",")
        comaPos2 = data.find(",",comaPos + 1)
        comaPos3 = data.find(",", comaPos2 + 1)
        dashPos = data.find("\\")

        temperature = data[aposPos + 1: comaPos]
        pressure = data[comaPos + 2: comaPos2]
        altitude = data[comaPos2 + 2: comaPos3]
        humidity = data[comaPos3 + 2: dashPos]

        # printing values (variables)
        print(f'|temperature = {temperature}| pressure = {pressure}| altitude = {altitude}| humidity = {humidity}|')

