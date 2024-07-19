#include"kalmanFilter.h"
//定义卡尔曼结构体并初始化参数
KFP KFP_Init;
void kalman_Init()
{
    KFP_Init.Kg = 0;//卡尔曼增益 初始化值为0
    KFP_Init.Last_Out = 0;//卡尔曼滤波器上次输出 初始化值为0
    KFP_Init.Last_P = 0;//上次估算协方差 初始化值为0
    KFP_Init.Now_Out = 0;//卡尔曼滤波器本次输出 初始化值为0
    KFP_Init.Now_P = 0;//当前估算协方差 初始化值为0
    KFP_Init.Out_Mid = 0;//卡尔曼滤波器状态预测值 初始化值为0
    KFP_Init.P_Mid = 0;//卡尔曼滤波器协方差预测值 初始化值为0
    KFP_Init.Q = 0.001;//初始化值为0.001,过程噪声协方差,Q增大，动态响应变快，收敛稳定性变坏
    KFP_Init.R = 0.534;//初始化值为0.543,测量噪声协方差,R增大，动态响应变慢，收敛稳定性变好
}
/**
 *卡尔曼滤波器
 *@param KFP *KFP 卡尔曼结构体参数
 *@float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float kalmanFilter(KFP* KFP, float input)
{
    //基于上次的最优估计值，预测K时刻的状态值(1)------------无控制输入
    KFP->Out_Mid= KFP->Last_Out
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差(2)
    KFP->P_Mid = KFP->Last_P + KFP->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）(3)
    KFP->Kg = KFP->P_Mid / (KFP->P_Mid + KFP->R);
    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）(4)
    KFP->Now_Out = KFP->Out_Mid + KFP->Kg * (input - KFP->Out_Mid);
    //更新协方差方程(5)
    KFP->Now_P = (1 - KFP->Kg) * KFP->P_Mid;
    //本次的系统协方差赋给KFP->LastP 为下一次运算准备
    KFP->Last_P = KFP->Now_P;
    //本次的最优值赋给KFP->Last_Out 为下一次运算准备
    KFP->Last_Out = KFP->Now_Out;
    return KFP->Now_Out;
}

/**
 *调用卡尔曼滤波器 实践
 */
float value;
int kalman_value = 0;
kalman_value = kalmanFilter(&KFP_Init, value);
