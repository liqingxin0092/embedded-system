#include "kem.h"

void Kalman_Init(Kalman_pm_st* Kalman_pm)
{
/*ע�⣺ֻ��Ҫ����Q����[Q_angle,Q_gyro]��R����[R_angle]����*/
 
    Kalman_pm->P[0][0] = 1.0f;
    Kalman_pm->P[0][1] = 0.0f;
    Kalman_pm->P[1][0] = 0.0f;
    Kalman_pm->P[1][1] = 1.0f;

    Kalman_pm->dt = 0.01f;      //ʱ��
    
    Kalman_pm->Q_angle = 0.002f;   //ԭ���� 0.001  //����ֵ�Ĳ�����С
    Kalman_pm->Q_gyro = 0.0035f;    //ԭ���� 0.003  //ֵ�����Ĳ�����С
    Kalman_pm->R_angle = 0.6f;     //ԭ���� 0.5     //�۲�Ĳ�����С
    
    Kalman_pm->q_bias = 0.0f;
    Kalman_pm->angle_f = 0.0f;
}
float Kalman_Filter(Kalman_pm_st *kalman_pm,float ValueForRepair,float D_Value)
{
    float angle_err;  //�������
    float angle_;     //�������
    float Pdot[2][2]; //�������Э�������
 
    float K_0;
    float K_1; //����������
 
    /*��ֵ*/
    kalman_pm->angle_m= ValueForRepair;   //ֵ
    kalman_pm->wb_m= D_Value;      //ֵ�ĵ���
    /*�������*/
    angle_ = kalman_pm->angle_f + (kalman_pm->wb_m - kalman_pm->q_bias) * kalman_pm->dt; //�������
    angle_err = kalman_pm->angle_m - angle_;                                             //�������
 
    /*�������Э�������*/
    Pdot[0][0] = kalman_pm->P[0][0] + kalman_pm->Q_angle - (kalman_pm->P[0][1] + kalman_pm->P[1][0]) * kalman_pm->dt; // Q_angle->Q1
    Pdot[0][1] = kalman_pm->P[0][1] - (kalman_pm->P[1][1]) * kalman_pm->dt;
    Pdot[1][0] = kalman_pm->P[1][0] - (kalman_pm->P[1][1]) * kalman_pm->dt;
    Pdot[1][1] = kalman_pm->P[1][1] + kalman_pm->Q_gyro; // Q_gyro->Q2
 
    /*����������*/
    K_0 = Pdot[0][0] / (Pdot[0][0] + kalman_pm->R_angle);
    K_1 = Pdot[1][0] / (Pdot[0][0] + kalman_pm->R_angle);
 
    /*�������*/
    kalman_pm->angle_f = angle_ + K_0 * angle_err;         //����ֵ
    kalman_pm->q_bias += K_1 * angle_err;                  //���ŵ���ƫ��
    kalman_pm->wb_f = kalman_pm->wb_m - kalman_pm->q_bias; //���ŵ���
 
    /*�������Э�������*/
    kalman_pm->P[0][0] = Pdot[0][0] - K_0 * Pdot[0][0];
    kalman_pm->P[0][1] = Pdot[0][1] - K_0 * Pdot[0][1];
    kalman_pm->P[1][0] = Pdot[1][0] - K_1 * Pdot[0][0];
    kalman_pm->P[1][1] = Pdot[1][1] - K_1 * Pdot[0][1];
    
    return kalman_pm->angle_f;
}

