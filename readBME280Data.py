import serial
import matplotlib.pyplot as plt
import numpy as np
import mariadb
import sys

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

with serial.Serial("com8", "9600") as cansatConn:
    with conn.cursor() as cur:

        while True:
            if (cansatConn.inWaiting()>0):
                data = str(cansatConn.readline())
                data = data.replace("\n", "").replace("\\n", "").replace("b", "").replace(" ", "")
                data = data.split(",")
                datareplace = data[0].replace("'", "")
                data.pop(0)
                data.insert(0, datareplace)
                data.pop(len(data)-1)
                if len(data) == 12:
                    print(data)
                    
                    

                    cur.execute(
                    "INSERT INTO datatable1 (temperature, pressure, altitude, humidity, uva_raw, uvb_raw, comp1_raw, comp2_raw, uva, uvb, uvi, uvi_mwpcm2) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
                    (data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11]))

                    conn.commit()
                    
                else:
                    print("Please wait recalibrating")

                


