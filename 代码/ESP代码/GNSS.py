from machine import UART
import time
uart2 = UART(2, 9600)                         # init with given baudrate
uart2.init(9600, bits=8, parity=None, stop=1) # init with given parameters

uart2.write("$PCAS03,1,0,0,0,0,0,0,0,0,0,,,0,0*03\r\n")

while True:
    time.sleep_ms(200)
    a=uart2.read()
    if a!=None:
        print(a.decode("utf-8"))
