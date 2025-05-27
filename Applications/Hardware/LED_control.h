/**
 * @file LED_control.h
 * @author sethome 
 * @brief ����LED����ͷ�ļ�
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "stm32f4xx_hal.h"
#include <stdint.h>
#ifndef LED_CONTROL_H
#define LED_CONTROL_H

//Ԥ����ɫ��
#define RED    0xFFFF0000
#define GREEN  0xFF00FF00
#define BLUE   0xFF0000FF
#define YELLOW 0xFFFFFF00
#define PINK   0xFFFFC0CB
#define ORANGE 0xFFDA6E00
#define PURPLE 0xFF800080
#define BLANK  0xFF000000

//�ⲿ����
void led_init(void);          //LED�ʵƳ�ʼ��
void led_show(uint32_t aRGB); //�趨����ɫ
	
#endif
