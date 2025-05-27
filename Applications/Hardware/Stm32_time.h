/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-11-06 23:58:28
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-07 00:01:04
 * @FilePath: /25_EE_omni_sentry/Applications/Hardware/Stm32_time.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __STM32_TIME_H
#define __STM32_TIME_H

#include "stm32f4xx_hal.h"
#include "stdint.h"

// ��ȡϵͳʱ��
uint32_t Get_sys_time_ms(void); // recommend
uint32_t Get_sys_time_us(void);

float Get_sys_time_s(void);

void TIM_count_100KHz(void);
#endif
// end of file