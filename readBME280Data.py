import serial
import matplotlib.pyplot as plt
import numpy as np

T = []
P = []
A = []
H = []

fig = plt.figure()
t_ax = fig.add_subplot(221, title="Temperature graph")
p_ax = fig.add_subplot(222, title="Pressure graph")
a_ax = fig.add_subplot(223, title="Altitude graph")
h_ax = fig.add_subplot(224, title="Humidity graph")
plt.tight_layout()

cansatConn = serial.Serial("com8", "9600")

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

        T.append(float(temperature))
        P.append(float(pressure))
        A.append(float(altitude))
        H.append(float(humidity))
        
        t_ax.plot(np.arange(len(T)), T, color="blue")
        p_ax.plot(np.arange(len(P)), P, color="blue")
        a_ax.plot(np.arange(len(A)), A, color="blue")
        h_ax.plot(np.arange(len(H)), H, color="blue")
        
        plt.pause(0.001)
        
