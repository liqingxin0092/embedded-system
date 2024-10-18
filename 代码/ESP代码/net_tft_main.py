import time
import network
import socket
import ST7735
from ST7735 import tft
import sysfont

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

def main():
    
    do_connect()#连接 wifi

    s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)#创建套接字
    
    s.bind(("0.0.0.0",7788))#绑定端口
    raw=-7
    while True:
        recv_msg=s.recvfrom(1024)#括号里的是接收个数,这个值上限是1024
        #接收的内容是一个元组(内容,("ip",端口号))
        
        print(recv_msg)
        raw=raw+7
        if raw==161:
            raw=0
            tft.fill(tft.WHITE)
        ST7735.tft.text((0,raw),recv_msg[0].decode("utf-8"),tft.BLACK,sysfont.sysfont,1.1)

        #发送要编码,接收要解码, b"xxxx" b意思是二进制.对recv_msg进行解包
#         print("%s(%d)>>>%s" % (recv_msg[1][0],recv_msg[1][1],recv_msg[0].decode("utf-8")))


    s.close()

    print('end')
    
if __name__ == "__main__":
    main()