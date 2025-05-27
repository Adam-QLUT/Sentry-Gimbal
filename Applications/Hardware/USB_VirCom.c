/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-11-18 18:10:06
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-18 18:10:21
 * @FilePath: /25_EE_omni_sentry/Applications/Hardware/USB_VirCom.c
 * @Description: 
 */
/**
 * @file USB_VirCom.c
 * @author sethome
 * @brief 虚拟串口数据发送
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "usbd_cdc_if.h"
#include "USB_VirCom.h"
#include "Stm32_time.h"
#include "global_status.h"
#include "NUC_communication.h"
//bool if_vision_start =0;
void VirCom_send(uint8_t data[], uint16_t len)
{
  CDC_Transmit_FS(data, len);
}

void VirCom_rev(uint8_t data[], uint16_t len)
{
  if(data[0]==0xA5){
    Global.Auto.input.Auto_control_online=1;
    decodeMINIPCdata(&fromMINIPC,data,len);
    MINIPC_to_STM32();
  }
}