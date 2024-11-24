import time
from  ST7735 import TFT 
from machine import UART,SPI,Pin
import sysfont
import network
import socket
import urequests
import json

weidu=0.0
jingdu=0.0
net_geo="https://restapi.amap.com/v3/geocode/geo?key=a1d091aa4c1d39606621e74263e68976&address="

def geo(address1:str)->dict:
    temp=net_geo+address1
    response = urequests.get(temp)
    return json.loads(response.text)['geocodes'][0]['location']

def trans(address2:str)->str:   
#     if weidu!=0.0 and jingdu!=0.0:
    temp= '{:.6f},{:.6f}'.format(jingdu,weidu)
    return "https://restapi.amap.com/v3/direction/walking?origin={}&destination={}&key=a1d091aa4c1d39606621e74263e68976".format(temp,geo(address2))

s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #创建套接字
    
def route(destination:str)->dict:
    a=trans(destination) #获取网址
    res2=urequests.get(a) #发送请求
    jd2=json.loads(res2.text) #开始解析
    coord2=jd2['route']['paths'][0]['steps']
    len_1=len(coord2)
    c=str('路程规划共分为'+str(len_1)+'步'+'如下:')
    s.sendto(c.encode("utf-8"),('192.168.43.68',7890))
    for i in range(0,len_1):
        b=coord2[int(i)]['instruction']
        s.sendto(b.encode("utf-8"),('192.168.43.68',7890))
        
def do_connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    if not wlan.isconnected():
#         print('connecting to network...')
        wlan.connect('vivopxd', '14725836')
        i=1
        while not wlan.isconnected():
#             print("正在连接网络...{}".format(i))
            tft.text((0,0),'Connecting...{}'.format(i),tft.YELLOW,sysfont.sysfont,1)
            time.sleep(1)
            i=i+1
            if i>8:
                tft.fillrect((0,0), (128,8), tft.BLACK)
                tft.text((0,0),'Network:failed',tft.RED,sysfont.sysfont,1)
                return ;
#     print('连接成功!network config:', wlan.ifconfig())
    tft.fillrect((0,0), (128,8), tft.BLACK)
    tft.text((0,0),'Network:succeed',tft.GREEN,sysfont.sysfont,1)

spi = SPI(2, baudrate=10000000, polarity=0, phase=0, sck=Pin(18), mosi=Pin(23))
BL = Pin(22, Pin.OUT)    
BL.value(1)
tft=TFT(spi,2,15,5) #初始化TFT
tft.initr()
tft.rgb(False)
tft.fill(tft.BLACK)
    
def main():
    
    uart2 = UART(2, 9600)                         # init with given baudrate
    uart2.init(9600, bits=8, parity=None, stop=1) # init with given parameters
    
    do_connect() #联网
    
    uart2.write("$PCAS03,0,0,0,0,1,0,0,0,0,0,,,0,0*03\r\n") #初始化GNSS
    
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)#初始化套接字
    s.bind(("192.168.43.134",7890))
    s.setblocking(False)
    
    i=0
    while True:
        
        a=uart2.read() #串口接收GNSS数据
        
        try:           #网络接收信息
            recv_content,client_info = s.recvfrom(1024)
            if recv_content:
#                 print("Received:", recv_content)
                temp=recv_content.decode("utf-8")
                tft.fillrect((0,32), (128,8), tft.BLACK)
                tft.text((0,32),'msg:{}'.format(temp),tft.YELLOW,sysfont.sysfont,1)
                if temp=='trace':
                    s.sendto('esp32:你想去哪?'.encode("utf-8"),('192.168.43.68',7890))
                    s.setblocking(True) #开启阻塞
                    recv_content,client_info = s.recvfrom(1024)
                    s.setblocking(False)#结束阻塞
                    tft.text((0,40),'analysing...',tft.BLUE,sysfont.sysfont,1)
                    c=recv_content.decode("utf-8")
#                     print(trans(c))
                    route(c) #导航
                    tft.fillrect((0,40), (128,8), tft.BLACK)
        except OSError as e:
            pass
            
        tft.text((0,144),'run..{}'.format(i),tft.BLUE,sysfont.sysfont,1)#指示运行中
        i=i+1
        
        if a!=None: #如果串口接收到数据
            b=a.decode("utf-8") #切片开始解析
            c=b.split(",")
            print(a.decode("utf-8"))
            
            
            if c[2]=='V': #如果GNSS数据无效
                tft.fillrect((0,8), (128,24), tft.BLACK)
                tft.text((0,8),'GNSS:failed',tft.RED,sysfont.sysfont,1)
                tft.text((0,16),'weidu:--',tft.RED,sysfont.sysfont,1)
                tft.text((0,24),'jingdu:--',tft.RED,sysfont.sysfont,1)
                
                if c[9]:
                    e=int(c[9])       #解析年月日
                    year=e%100
                    month=e//100%100
                    date=e//10000
                    
                    e=float(c[1])       #解析时分秒
                    hour=int(e//10000)+8
                    minut=int(e//100%100)
                    sec=int(e%100)
                    
                    tft.fillrect((0,152), (128,8), tft.BLACK)
                    tft.text((0,152),'{:0>2d}/{:0>2d}/{:0>2d}-{:0>2d}:{:0>2d}:{:0>2d}'.format(year,month,date,hour,minut,sec),tft.YELLOW,sysfont.sysfont,1)
                else:
                    tft.fillrect((0,152), (128,8), tft.BLACK)
                    tft.text((0,152),'tiem get failed',tft.RED,sysfont.sysfont,1)
            else :
                e=float(c[3])          #解析经纬度
                global weidu
                weidu=e%100/60+e//100
                
                e=float(c[5])
                global jingdu
                jingdu=e%100/60+e//100
                
                e=int(c[9])       #解析年月日
                year=e%100
                month=e//100%100
                date=e//10000
                
                e=float(c[1])       #解析时分秒
                hour=int(e//10000)+8
                minut=int(e//100%100)
                sec=int(e%100)

                tft.fillrect((0,8), (128,24), tft.BLACK)
                tft.text((0,8),'GNSS:succeed',tft.GREEN,sysfont.sysfont,1)
                tft.text((0,16),'weidu:{:.6f}'.format(weidu),tft.GRAY,sysfont.sysfont,1)
                tft.text((0,24),'jingdu:{:.6f}'.format(jingdu),tft.GRAY,sysfont.sysfont,1)
                tft.text((0,152),'{:0>2d}/{:0>2d}/{:0>2d}-{:0>2d}:{:0>2d}:{:0>2d}'.format(year,month,date,hour,minut,sec),tft.YELLOW,sysfont.sysfont,1)
                
        time.sleep_ms(500) 
if __name__ == "__main__":
    main()