import serial
ser = serial.Serial('COM5', 115200)

while 1:
    try:
        file = open('Transfer.txt', 'w')
        file.write("")
        file.write(ser.readline())
        file.close()
        print ser.readline()
    except: IOError

