/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-11-18 18:10:06
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-18 18:12:54
 * @FilePath: /25_EE_omni_sentry/Applications/Hardware/USB_VirCom.h
 * @Description: 
 */
/**
 * @file USB_VirCom.h
 * @author sethome
 * @brief 虚拟串口数据发送
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */
#define __USB_VIR_COM_H__
#ifdef __USB_VIR_COM_H__

#include "struct_typedef.h"
#include "stdint.h"
#include "stdbool.h"
void VirCom_send(uint8_t data[],uint16_t len);
void VirCom_rev(uint8_t data[],uint16_t len);
//extern bool if_vision_start ;
#endif
//end of file
