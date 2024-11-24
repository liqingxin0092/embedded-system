import time
from  ST7735 import TFT,TFTColor
from machine import UART,SPI,Pin
import sysfont
import network
import socket
import urequests
import json


def do_connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('vivopxd', '14725836')
        i=1
        while not wlan.isconnected():
            print("正在连接网络...{}".format(i))
            tft.text((0,0),'Connecting...{}'.format(i),tft.YELLOW,sysfont.sysfont,1)
            time.sleep(1)
            i=i+1
            if i>8:
                tft.fillrect((0,0), (128,8), tft.BLACK)
                tft.text((0,0),'Network:failed',tft.RED,sysfont.sysfont,1)
                return ;
    print('连接成功!network config:', wlan.ifconfig())

def main():
    spi = SPI(2, baudrate=30000000, polarity=0, phase=0, sck=Pin(18), mosi=Pin(23))
    BL = Pin(22, Pin.OUT)    
    BL.value(1)
    
    tft=TFT(spi,2,15,5)
    tft.initr()
    tft.rgb(False)#可以控制图片正反
    tft.fill(tft.BLACK)
    
    do_connect()#连WIFI

    server_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_s.bind(("", 7890))
    server_s.listen(128)
    print("等待连接...")
    new_s, client_info = server_s.accept()
    print("等待信息...")
    with open("output1.bmp", "wb") as f:
        while True:
            data = new_s.recv(100)
#             print(data)
            if data==''.encode("utf.8"):
                break
            else :
                f.write(data)
    print("接收完毕.")
    new_s.close()
    server_s.close()
####################
    f=open('output1.bmp', 'rb')
    if f.read(2) == b'BM':  #header
        dummy = f.read(8) #file size(4), creator bytes(4)
        offset = int.from_bytes(f.read(4), 'little')
        hdrsize = int.from_bytes(f.read(4), 'little')
        width = int.from_bytes(f.read(4), 'little')
        height = int.from_bytes(f.read(4), 'little')
        if int.from_bytes(f.read(2), 'little') == 1: #planes must be 1
            
            depth = int.from_bytes(f.read(2), 'little')
            print(depth)
            if depth == 24 and int.from_bytes(f.read(4), 'little') == 0:#compress method == uncompressed
             
                print("Image size:", width, "x", height)
                rowsize = (width * 3 + 3) & ~3
                if height < 0:
                    height = -height
                    flip = False
                else:
                    flip = True
                w, h = width, height
                if w > 128: w = 128
                if h > 160: h = 160
                tft._setwindowloc((0,32),(w - 1,32+h - 1))
                for row in range(h):
                    if flip:
                        pos = offset + (height - 1 - row) * rowsize
                        
                    else:
                        pos = offset + row * rowsize
                    if f.tell() != pos:
                        dummy = f.seek(pos)
                     
                    for col in range(w):
                        bgr = f.read(3)
                        tft._pushcolor(TFTColor(bgr[0],bgr[1],bgr[2]))
        
if __name__ == "__main__":
    main()
