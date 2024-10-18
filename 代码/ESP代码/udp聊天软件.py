import time
import network
import socket
import ST7735
from ST7735 import tft
import sysfont


def send_msg(s):
    #1.发送
        #1.获取用户发送的数据内容
    send_content=input("请输入要发送的数据内容:")
        #2.获取目的的IP
    send_ip=input("请输入要发送的目标IP:")
        #3.获取目的PORT
    send_port=int(input("请输入要发送的目标PORT:"))
        #4.sendto
    s.sendto(send_content.encode("utf-8"),(send_ip,send_port))
    
def recv_msg(s):
    recv_content,client_info =s.recvfrom(1024)
    print(">>>%s(%d):%s"%(client_info[0],client_info[1],recv_content.decode("utf-8")))


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
    
    print("abcd")    
    do_connect()#联网
    #1.创建套接字
    s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    #2.绑定信息
    s.bind(("",7890))
    #3.功能选择


    #2.接收
        #1.recv_from接收数据
        #2.打印数据
    while True :
        print("1.发送数据")
        print("2.接收数据")
        print("3.退出")
        op=input("要进行的操作序号:")
        if op=='1':
            send_msg(s)
        elif op == '2':
            recv_msg(s)
        elif op == '3':
            print("操作结束")
            s.close()
            break
        #3.关闭套接字
        
if __name__ == "__main__":
    main()