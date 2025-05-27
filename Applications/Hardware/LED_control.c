/**
 * @file LED_control.c
 * @author sethome (you@domain.com)
 * @brief ����LED����
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "LED_control.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim5;

//��ʼ��LED
void led_init()
{
	HAL_TIM_Base_Start(&htim5);               //������ʱ��5
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);  //����ÿһ��ͨ��
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
}

//��ʾ��ɫ
void led_show(uint32_t aRGB)
{
  static uint8_t alpha;
  static uint16_t red,green,blue;

  //λ�ƻ�ȡ��ɫ
  alpha = (aRGB & 0xFF000000) >> 24;
  red = ((aRGB & 0x00FF0000) >> 16) * alpha;
  green = ((aRGB & 0x0000FF00) >> 8) * alpha;
  blue = ((aRGB & 0x000000FF) >> 0) * alpha;

  //ʹ�ܱȽ�ֵ
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);  
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}
