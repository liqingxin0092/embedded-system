/*
 * motor_control.c
 *
 *  Created on: Feb 28, 2024
 *      Author: Lenovo
 */
#include "motor_control.h"
#include "main.h"
#include "pid.h"
extern TIM_HandleTypeDef htim1;
extern tPid pidMotor1Speed;
extern tPid pidMotor2Speed;
extern float speed1;
extern float speed2;

void motor_set(int motor1,int motor2)
{
	if(motor1>0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET );
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET );
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET );
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET );
	}

	if(motor2>0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET );
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET );
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET );
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
	}


	if(motor1<0)
	{
		if(motor1<-99) motor1=-99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,-motor1);
	}
	else
	{
		if(motor1>99) motor1=99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,motor1);
	}


	if(motor2<0)
	{
		if(motor2<-99) motor2=-99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4,-motor2);
	}
	else
	{
		if(motor2>99) motor2=99;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4,motor2);
	}
}

void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed)
{
//改变电机PID参数的目标速度
	pidMotor1Speed.target_val = Motor1SetSpeed;
    pidMotor2Speed.target_val = Motor2SetSpeed;
//根据PID计算 输出作用于电机
motor_set(PI_realize(&pidMotor1Speed,speed1),PI_realize(&pidMotor2Speed,speed2));

}

