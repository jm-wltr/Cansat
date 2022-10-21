import serial
import matplotlib.pyplot as plt
import numpy as np
import mariadb
import sys

T = []
P = []
H = []
A = []

fig = plt.figure()
t_ax = fig.add_subplot(221, title="Temperature graph", ylabel = "θ/ºC")
p_ax = fig.add_subplot(222, title="Pressure graph", ylabel = "p/hPa")
a_ax = fig.add_subplot(223, title="Altitude graph", ylabel = "A/m")
h_ax = fig.add_subplot(224, title="Relative humidity graph", ylabel = "%RH")
plt.tight_layout()

cansatConn = serial.Serial("com8", "9600")

try:
    conn = mariadb.connect(
        user = "root",
        password="Alt2020!",
        host="127.0.0.1",
        port = 3306,
        database="cansattest1"
        )

except mariadb.Error as e:
    print(f"Error onnecting to MariaDB Platform: {e}")
    sys.exit(1)

with conn.cursor() as cur:

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

            cur.execute(
            "INSERT INTO datatable1 (temperature, pressure, altitude, humidity) VALUES (?, ?, ?, ?);",
            (temperature, pressure, altitude, humidity))

            conn.commit() 

            T.append(float(temperature))
            P.append(float(pressure))
            A.append(float(altitude))
            H.append(float(humidity))

            t_ax.plot(np.arange(len(T)), T, color="blue")
            p_ax.plot(np.arange(len(P)), P, color="blue")
            a_ax.plot(np.arange(len(A)), A, color="blue")
            h_ax.plot(np.arange(len(H)), H, color="blue")

            plt.pause(0.001)

