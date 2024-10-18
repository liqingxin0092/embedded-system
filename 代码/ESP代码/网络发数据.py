import time
import network
import socket

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
    
    # 0. 连接 wifi
    do_connect()

    s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    
    s.bind(("0.0.0.0",7788))

    while True:
        msg=input("请输入要发送的数据:")
        s.sendto(msg.encode("utf-8"),("192.168.43.68",8080))

    s.close()

    print('end')
    
if __name__ == "__main__":
    main()