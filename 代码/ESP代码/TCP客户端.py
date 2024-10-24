import  socket
import network
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
    do_connect()
    #1.创建套接字(TCP)
    client_s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    #2.连接服务器
    client_s.connect(("192.168.43.68",8080))
    print("yes")
    #3.发送数据    #用的是send不是sendto.
    while True:
        a=int(input("选择功能:1,发送.2,退出."))
        if a==1:
            client_s.send("hello pxd".encode("utf-8"))
        elif a==2:
            break
    #3.接收数据    #用的是recv不是recvfrom.
#     a=client_s.recv(1024).decode("utf-8")
#     print(a)
    #4.关闭
    client_s.close()

if __name__=="__main__":
    main()