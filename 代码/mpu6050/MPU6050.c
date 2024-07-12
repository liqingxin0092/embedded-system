#include "MPU6050.h"
#include "math.h"
uint8_t Test;
//�ı䷢�͵�ַ����0xd0��ַ�£�a=0˵��Ӧ��1δӦ��
void TestMPU6050()
{
  start();
  send(MPU6050_ADDRESS);        //1101 000 0,ǰ7λ��MPU6050�ĵ�ַ,���һλ��Ϊд����
  Test=receive_ack();
  stop();   
}

void MPU6050Init()
{
    WriteReg(MPU6050_ADDRESS,MPU6050_PWR_MGMT_1,1);//���˯�ߣ��¶ȴ�������ʧ�ܣ�ѡ��x��������ʱ��
    WriteReg(MPU6050_ADDRESS,MPU6050_SMPLRT_DIV,0x09);//10��Ƶ
    WriteReg(MPU6050_ADDRESS,MPU6050_CONFIG,0x06);  //��ƽ����ͨ�˲�
    WriteReg(MPU6050_ADDRESS,MPU6050_GYRO_CONFIG,0x10);//1000��/s
    WriteReg(MPU6050_ADDRESS,MPU6050_ACCEL_CONFIG,0x10);//8g     
}

void GetData(SixAxisData * data)
{
    uint8_t DataBuffer[14];
    ReadRegs(MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_H,DataBuffer,14);
    data->AccX= (int16_t)((DataBuffer[0]<<8)+DataBuffer[1])/32768.0*8;
    data->AccY= (int16_t)((DataBuffer[2]<<8)+DataBuffer[3])/32768.0*8;
    data->AccZ=  (int16_t)((DataBuffer[4]<<8)+DataBuffer[5])/32768.0*8;  
    data->GyroX= (int16_t)((DataBuffer[8]<<8)+DataBuffer[9])/32768.0*1000;  
    data->GyroY= (int16_t)((DataBuffer[10]<<8)+DataBuffer[11])/32768.0*1000; 
    data->GyroZ= (int16_t)((DataBuffer[12]<<8)+DataBuffer[13])/32768.0*1000;
    data->Temperature=(int16_t)((DataBuffer[6]<<8)+DataBuffer[7])/340.0+36.53;
}
void GetAngle(Angle * angle,SixAxisData * data)
{
    angle->roll=  180*atan(data->AccY/data->AccZ)/3.14;
    angle->pitch= 180* atan((float)data->AccX/sqrt(data->AccY*data->AccY+data->AccZ*data->AccZ))/3.14;
}
