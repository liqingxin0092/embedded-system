import time
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
            time.sleep(1)
            i=i+1
    print('连接成功!network config:', wlan.ifconfig())

net_geo="https://restapi.amap.com/v3/geocode/geo?key=a1d091aa4c1d39606621e74263e68976&address="
net_walk='https://restapi.amap.com/v3/direction/walking?origin=116.434307,39.90909&destination=116.434446,39.90816&key=<用户的key>'
#参数是字符串,返回值是字典
#功能是获取一个地方的经纬度

def geo(address1:str)->dict:
    temp=net_geo+address1
    response = urequests.get(temp)
    return json.loads(response.text)['geocodes'][0]['location']

def trans(address1:str,address2:str)->str:
    return "https://restapi.amap.com/v3/direction/walking?origin={}&destination={}&key=76e3f7d25c73a24572235216729b2ef2".format(geo(address1),geo(address2))

s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

def route(origin:str,destination:str)->dict:
    a=trans(origin,destination)
    res2=urequests.get(a)
    jd2=json.loads(res2.text)
    coord2=jd2['route']['paths'][0]['steps']
    len_1=len(coord2)
    c=str('路程规划共分为'+str(len_1)+'步'+'如下:')
    s.sendto(c.encode("utf-8"),('192.168.43.68',7890))
    for i in range(0,len_1):
        b=coord2[int(i)]['instruction']
        s.sendto(b.encode("utf-8"),('192.168.43.68',7890))

def main():   
    #联网
    do_connect()
    #创建套接字
    #2.绑定信息
    s.bind(("",7890))
    while True:
        print("等待消息中...")
        recv_content,client_info =s.recvfrom(1024)
        msg=recv_content.decode("utf-8")
        print(">>>%s(%d):%s"%(client_info[0],client_info[1],msg))
        if msg=='获取编码':
            s.sendto('esp32:准备获取编码....'.encode("utf-8"),('192.168.43.68',7890))
            s.sendto('esp32:选择你要查询的地方:'.encode("utf-8"),('192.168.43.68',7890))
            a=s.recvfrom(1024)[0].decode("utf-8")#获取电脑的输入值
            b=geo(a)#获取到地理编码
            s.sendto(b.encode("utf-8"),('192.168.43.68',7890))
        elif  msg=='我要导航':
            s.sendto('esp32:你目前在哪:'.encode("utf-8"),('192.168.43.68',7890))
            a=s.recvfrom(1024)[0].decode("utf-8")#获取电脑地址1
            
            s.sendto('esp32:你想去哪:'.encode("utf-8"),('192.168.43.68',7890))
            c=s.recvfrom(1024)[0].decode("utf-8")#获取电脑地址2
            
            route(a,c)
            
        else :
            s.sendto("esp32:ack".encode("utf-8"),('192.168.43.68',7890))
    #关套接字
    s.close()
        
#     print(geo("河北科技大学"))

if __name__ == "__main__":
    main()