/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include <MATH.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ADC_Value_Limit 3400
#define Touch(M)   (adc_value[M] > ADC_Value_Limit)
#define UnTouch(M)  (adc_value[M] < ADC_Value_Limit)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t adc_value[4];
int16_t angle;
enum  MIC
{
	M1,
	M2,
	M3,
	M4,
};
enum  TIM
{
	tim1,
	tim2,
	tim3,
	tim4,
};
double p, q;
typedef struct
{
	double x;
	double y;
}Position;
Position pos = {0};
typedef struct
{
	uint8_t  TouchFlag;
	double time;
}MicToTime;
MicToTime arr[4] = {0};
double StdTime ;
double a1,a2;
double b1,b2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	if(htim==&htim8)
	{
		StdTime++;

		if (Touch(M1)&&arr[tim3].TouchFlag == 0)
		{
			arr[tim3].TouchFlag = 1;
			arr[tim3].time = StdTime;
		}
		else if (Touch(M2)&&arr[tim4].TouchFlag == 0) //对
		{
			arr[tim4].TouchFlag = 1;
			arr[tim4].time =StdTime;
		}
		else if (Touch(M3)&&arr[tim1].TouchFlag == 0)
		{
			arr[tim1].TouchFlag = 1;
			arr[tim1].time = StdTime;
		}
		else if (Touch(M4)&&arr[tim2].TouchFlag == 0)   //对
		{
			arr[tim2].TouchFlag = 1;
			arr[tim2].time = StdTime;
		}
	}

 }

void  Calculate_hyperbola()
{
	a1 = 170 * 0.00000761905*(arr[tim1].time - arr[tim4].time);

	b1 = sqrt(0.09-a1*a1);

	a2 = 170 * 0.00000761905*(arr[tim2].time - arr[tim3].time);

	b2 = sqrt(0.09 - a2 * a2);
	if (b1 * b2 > a1 * a2)
	{
		p = 100*sqrt(a1 * a1 * b2 * b2 * (a2 * a2 + b1 * b1) / (b1 * b1 * b2 * b2 - a1 * a1 * a2 * a2));
		q = 100*sqrt(a2 * a2 * b1 * b1 * (a1 * a1 + b2 * b2) / (b1 * b1 * b2 * b2 - a1 * a1 * a2 * a2));
	}
	else
	{
		p = q = 0;
	}

	if (a2 > 0)       //来回试一试
		pos.y = -q;
	else
		pos.y =q;
	if (a1 > 0)
		pos.x =-p;
	else
		pos.x =p;

	angle = 180/3.1415926*atan(pos.y/pos.x);
	if(angle<0)
			angle+=180;
		if(angle>180)
				angle-=180;
}

void ALL_Init()
{
	memset(arr, 0, sizeof(arr));
	memset(&pos, 0, sizeof(pos));
}
void Servo_SetAngle(float Angle)
{
	 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Angle / 180 * 2000 + 500);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM8_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim8);     //ADC定时采样的定时器

  OLED_Init();
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_value, 4);
  Servo_SetAngle(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		ALL_Init(); //初始化
	/*    while (UnTouch(0) && UnTouch(1) && UnTouch(2) && UnTouch(3));    //有一个触发开启循环*/

	  	OLED_ShowNum(80, 32, adc_value[M1]/100, 2,OLED_8X16);
		OLED_ShowNum(100, 32, adc_value[M2]/100, 2,OLED_8X16);
		OLED_ShowNum(80, 48, adc_value[M3]/100, 2,OLED_8X16);
		OLED_ShowNum(100, 48, adc_value[M4]/100, 2,OLED_8X16);

		OLED_ShowNum(110, 0,arr[M1].TouchFlag + arr[M2].TouchFlag + arr[M3].TouchFlag + arr[M4].TouchFlag,2,OLED_8X16);
		OLED_Update();           //显示触发几个


		if(arr[M1].TouchFlag + arr[M2].TouchFlag + arr[M3].TouchFlag + arr[M4].TouchFlag!=4)//如果没触发四个直接初始化并且跳过
		{
			ALL_Init();
			continue;
		}
		else   //如果触发4个就计算
	    {

		  Calculate_hyperbola();     //计算

			if(pos.x>0&&pos.y>0) //一项限                    //亮灯
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 , 1);
			}
			else if(pos.x>0&&pos.y<0) //四象限
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3 , 1);
			}
			else if(pos.x<0&&pos.y>0)   //二象限
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1 , 1);
			}
			else if(pos.x<0&&pos.y<0)  //三象限
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 , 1);
			}

			  OLED_ShowString(0, 0, "x", OLED_8X16	);
			  OLED_ShowFloatNum(14, 0, pos.x, 3, 2,OLED_8X16);
			  OLED_ShowString(0, 16, "y", OLED_8X16	);
			  OLED_ShowFloatNum(14, 16, pos.y, 3, 2,OLED_8X16);

			  OLED_ShowFloatNum(0, 32, arr[tim1].time - arr[tim4].time,1 ,5,OLED_8X16);
			  OLED_ShowFloatNum(0, 48, arr[tim2].time - arr[tim3].time, 1,5,OLED_8X16);

			  OLED_ShowNum(75, 0, angle,4,OLED_8X16);

			  OLED_Update();         //显示结果

			  Servo_SetAngle(angle);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 , 0);

	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
