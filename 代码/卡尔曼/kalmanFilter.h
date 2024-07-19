#ifndef __KALMANFILTER_H
#define __KALMANFILTER_H

//结构体类型定义
typedef struct
{
    float Last_P;
    float Now_P;
    float Now_Out;
    float Last_Out;
    float Out_Mid;
    float P_Mid;
    float Kg;
    float Q;
    float R;
}KFP;//Kalman Filter parameter

extern KFP KFP_Init;
void kalman_Init();
float kalmanFilter(KFP* KFP, float input);
#endif
