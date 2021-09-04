import serial
import time


ser = serial.Serial(
    port='COM12',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=0)

print("connected to: " + ser.portstr)
count=0
print(ser.name)
res=0
text = 'q'
while True:
    # ser.write("{}\n\r".format(count).encode('utf-8'))
    ser.write("{}:hello world\n".format(count).encode('utf-8'))
    time.sleep(1)
    # for line in ser.read():
    if ser.in_waiting > 0:
        # print("current_byte_size:{}".format())
        if ser.readline()=='hoang\n'.encode('utf-8') :
            res = 1
            print(type(res))
        # print(str(count) + str(': ') + chr(line) )

    count = count+1
    if  res == 1:
        ser.close()
        break
ser.close()