from machine import Pin, SPI
from ST7735 import TFT,TFTColor
import sysfont
import network
import socket
import time

def do_connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('vivopxd', '14725836')
        i=1
        while not wlan.isconnected():
            print("正在连接网络...{}".format(i))
            time.sleep(1)
            i=i+1
    print('连接成功!network config:', wlan.ifconfig())

do_connect()

s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
s.bind(("192.168.43.134",7890))
s.setblocking(False)

while True:
     print("yes")
     try:
         recv_content,client_info = s.recvfrom(1024)
         if recv_content:
             print("Received:", recv_content)
             print("From:", client_info)
     except OSError as e:
             pass
     print("yes")      
     time.sleep_ms(1000)

# recv_content,client_info =s.recvfrom(1024)
# print(">>>%s(%d):%s"%(client_info[0],client_info[1],recv_content.decode("utf-8")))