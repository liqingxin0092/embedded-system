#ifndef __KALMAN_H_
#define __KALMAN_H_

#include "main.h"
#include "i2c.h"
#include "MPU6050.h"

// ʹ�÷�����
//       ��������ʼ��
//   Kalman_pm_st  Kalman_pm1;   //����ṹ��
//   Kalman_Init(&Kalman_pm1);    //���ṹ�帳��ֵ���޸Ĳ���
//       ѭ����ʹ��
//   float num= Kalman_Filter(Kalman_pm_st *Kalman_pm1,float ������ֵ,float ������ֵ�ĵ���);
//   ��dt
//   ÿ��Ҫ����һ�����ݾ�Ҫ����һ���µĽṹ��
//   ������ݲ�����С������.c�ļ���void Kalman_Init(Kalman_pm_st* Kalman_pm)������Q Q R

typedef struct
{
    float dt;      // ����ʱ��
    float angle_f; // ֵ�˲���
    float angle_m; // ֵ����
    float wb_m;    // ��������
    float wb_f;    // �����˲���
    float q_bias;  // ����offset
    float P[2][2]; // Э�������

    float Q_angle; // Q����
    float Q_gyro;

    float R_angle; // R����

} Kalman_pm_st;

void Kalman_Init(Kalman_pm_st *Kalman_pm);
float Kalman_Filter(Kalman_pm_st *kalman_pm, float ValueForRepair, float D_Value);

#endif /* KALMAN_H_ */
