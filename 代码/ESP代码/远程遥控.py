#大体流程
#1.esp32连WIFI,让其拥有ip地址.
#2.创建UDP socket
#3.接收UDP数据
#4.接收到的UDP数据控制LED亮灭.
import time
import network
from machine import Pin

def do_connect():
    '''
    作用:连接我的手机热点网络
    '''
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
    
def create_udp_socket():
    import socket
    #创建udp套接字
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #绑定一个固定的端口
    udp_socket.bind(("0.0.0.0",7788))
    
    return udp_socket

def main():
    p2 = Pin(2, Pin.OUT)
    #1.esp32连WIFI,让其拥有ip地址.
    do_connect()
    #2.创建UDP socket
    udp_socket=create_udp_socket()
    #3.接收UDP数据
    while True:
        recv_data,sender_info = udp_socket.recvfrom(1024)
        print("{}发送的数据:{}".format(sender_info,recv_data))
        recv_data_str=recv_data.decode("utf-8")
        print("解码后的数据:{}".format(recv_data_str))
        #4.接收到的UDP数据控制LED亮灭.
        if recv_data_str=="灯亮":
            p2.value(1)
        elif recv_data_str=="灯灭":
            p2.value(0)
if __name__ == "__main__":
    main()
