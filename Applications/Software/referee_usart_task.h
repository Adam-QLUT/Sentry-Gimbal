/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-12-20 20:35:17
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-20 20:37:41
 * @FilePath: /25_EE_omni_sentry/Applications/Software/referee_usart_task.h
 * @Description: 
 */
/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       referee_usart_task.c/h
  * @brief      RM referee system data solve. RM裁判系统数据处理
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
#ifndef REFEREE_USART_TASK_H
#define REFEREE_USART_TASK_H
#include "main.h"
#include "fifo.h"
#define USART_RX_BUF_LENGHT 512
#define REFEREE_FIFO_BUF_LENGTH 2048

/**
 * @brief          referee task
 * @param[in]      pvParameters: NULL
 * @retval         none
 */
extern fifo_s_t referee_fifo;
/**
 * @brief          裁判系统任务
 * @param[in]      pvParameters: NULL
 * @retval         none
 */
extern void referee_usart_task(void);
extern void referee_usart_task_init(void);
#endif
