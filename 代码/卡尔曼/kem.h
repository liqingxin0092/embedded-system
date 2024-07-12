#ifndef __KALMAN_H_
#define __KALMAN_H_

#include "main.h"
#include "i2c.h"
#include "MPU6050.h"

// 使用方法：
//       主函数初始化
//   Kalman_pm_st  Kalman_pm1;   //定义结构体
//   Kalman_Init(&Kalman_pm1);    //给结构体赋初值和修改参数
//       循环中使用
//   float num= Kalman_Filter(Kalman_pm_st *Kalman_pm1,float 待修正值,float 待修正值的导数);
//   改dt
//   每次要修正一个数据就要创建一个新的结构体
//   结合数据波动大小，调整.c文件中void Kalman_Init(Kalman_pm_st* Kalman_pm)函数的Q Q R

typedef struct
{
    float dt;      // 采样时间
    float angle_f; // 值滤波后
    float angle_m; // 值测量
    float wb_m;    // 导数测量
    float wb_f;    // 导数滤波后
    float q_bias;  // 导数offset
    float P[2][2]; // 协方差矩阵

    float Q_angle; // Q矩阵
    float Q_gyro;

    float R_angle; // R矩阵

} Kalman_pm_st;

void Kalman_Init(Kalman_pm_st *Kalman_pm);
float Kalman_Filter(Kalman_pm_st *kalman_pm, float ValueForRepair, float D_Value);

#endif /* KALMAN_H_ */
