# File:
# Author:   Edward Hanson
# Email:    ehanson1@umbc.edu

import serial
from time import sleep
ser = serial.Serial('COM5',9600)
sleep(1)

h = 90
v = 90
MAX = 120
MIN = 40

while h < MAX:
    h+= 1
    v+= 1
    ser.write('1 1;'.encode())
    sleep(0.01)
    
for i in range(2):
    while h > MIN:
        h -= 1
        ser.write('-1 0;'.encode())
        sleep(0.01)
    while v > MIN:
        v -= 1
        ser.write('0 -1;'.encode())
        sleep(0.01)
        
    while h < MAX:
        h += 1
        ser.write('1 0;'.encode())
        sleep(0.01)
    while v < MAX:
        v += 1
        ser.write('0 1;'.encode())
        sleep(0.01)

ser.write('a;'.encode())