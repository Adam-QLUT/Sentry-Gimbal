/**
 * @file NUC_communication.c
 * @brief
 * @author sethome (you@domain.com)
 * @version 1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "NUC_communication.h"
#include "UART_data_transmit.h"
#include "IMU_updata.h"
#include "global_status.h"
#include "USB_VirCom.h"
#include "Stm32_time.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "gimbal.h"
#include "gimbal.h"
#include "IMU_updata.h"
#include "CRC8_CRC16.h"
#include "gimbal.h"

#include "stdio.h"

#define EPISION 0.001f

// 导航使用uart1收发
STM32ROS_data_t stm32send_1;
STM32ROS_data_t stm32send_6;
STM32ROS_data_t stm32receive_1;
STM32ROS_data_t stm32receive_6;
Navigation_data_t Navigation_send_1;
Navigation_data_t Navigation_send_6;
Navigation_data_t Navigation_receive_1;
Navigation_data_t Navigation_receive_6;

float gimbal_scan_yaw = 0;
float gimbal_scan_pitch = 0;
int TurnOver_yaw = 0;
int TurnOver_pitch = 0;
long long WaitTime = 10;
float yaw_acc = 1;
int flagman = 0;
int flagdog = 0;
//
void Navigation_send_message()
{
	stm32send_1.remain_hp = robot_status.current_HP; // 现在的血量
	    stm32send_1.max_hp =robot_status.maximum_HP; //最大血量
//	stm32send_1.max_hp = 0;
//	stm32send_1.game_type = game_status.game_type; // ��������
		stm32send_1.game_type = 0; // ��������
	//	stm32send_1.game_progress = 4; //�����׶�
	stm32send_1.game_progress = game_status.game_progress;
	stm32send_1.stage_remain_time = game_status.stage_remain_time; // ��ǰ�׶�ʣ��ʱ��
	stm32send_1.bullet_remaining_num_17mm = projectile_allowance.projectile_allowance_17mm;
	stm32send_1.red_outpost_hp = game_robot_HP.red_outpost_HP;
	//	stm32send_1.red_base_hp = REFEREE_DATA.red_base_hp;
	stm32send_1.red_base_hp = game_robot_HP.red_base_HP;
  
	stm32send_1.blue_outpost_hp = game_robot_HP.blue_outpost_HP;
	//	stm32send_1.blue_base_hp = REFEREE_DATA.blue_base_hp;
	stm32send_1.blue_base_hp = REFEREE_DATA.blue_base_hp;
	stm32send_1.rfid_status = rfid_status.rfid_status;

	/* 自瞄传给导航 */
	stm32send_1.target_id = 0; // 目标id
	stm32send_1.target_pose[0] = 0;
	stm32send_1.target_pose[1] = 0;
	stm32send_1.target_pose[2] = 0;
	stm32send_1.target_vx = 0;	  // 目标x轴速度
	stm32send_1.target_vy = 0;	  // 目标y轴速度
	stm32send_1.target_vz = 0;	  // 目标z轴速度
	stm32send_1.target_theta = 0; // 装甲板朝向角
	stm32send_1.target_omega = 0; // 目标角速度

 static  char json[1024];
	int len = sprintf(json,
					  "{\"game_type\":%u,"
					  "\"game_progress\":%u,\"remain_hp\":%u,\"max_hp\":%u,\"stage_remain_time\":%u,\"bullet_remaining_num_17mm\":%u,"
					  "\"red_outpost_hp\":%u,\"red_base_hp\":%u,\"blue_outpost_hp\":%u,"
					  "\"blue_base_hp\":%u,\"rfid_status\":%u}\n",
					  /*stm32send_1.header,*/ stm32send_1.game_type,
					  stm32send_1.game_progress, stm32send_1.remain_hp, stm32send_1.max_hp, stm32send_1.stage_remain_time, stm32send_1.bullet_remaining_num_17mm,
					  stm32send_1.red_outpost_hp, stm32send_1.red_base_hp, stm32send_1.blue_outpost_hp,
					  stm32send_1.blue_base_hp, stm32send_1.rfid_status);
	HAL_UART_Transmit_DMA(UART1_data.huart, (uint8_t *)json, len);
}

// Auto control

STM32_data_t toMINIPC;
MINIPC_data_t fromMINIPC;

uint8_t data[128];
uint8_t rx_data[100];

void decodeMINIPCdata(MINIPC_data_t *target, unsigned char buff[], unsigned int len)
{
	memcpy(target, buff, len);
}

int encodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len)
{
	memcpy(tx_buff, target, sizeof(STM32_data_t));
	return 0;
}

void STM32_to_MINIPC()
{
    toMINIPC.FrameHeader.sof = 0xA5;
    toMINIPC.FrameHeader.crc8 = 0x00;

    toMINIPC.To_minipc_data.curr_pitch = IMU_data.AHRS.pitch;
    toMINIPC.To_minipc_data.curr_yaw = gimbal.yaw.encoder_rad;
    toMINIPC.To_minipc_data.curr_omega = cos(IMU_data.AHRS.pitch) * IMU_data.gyro[2] - sin(IMU_data.AHRS.pitch) * IMU_data.gyro[1];
    toMINIPC.To_minipc_data.autoaim = 1;
  if (
        
        robot_status.robot_id >= 100)
        toMINIPC.To_minipc_data.enemy_color = 1;
    else
        toMINIPC.To_minipc_data.enemy_color = 0;
  toMINIPC.To_minipc_data.state = 0;
    toMINIPC.FrameTailer.crc16 = get_CRC16_check_sum((uint8_t *)&toMINIPC.FrameHeader.sof, 17, 0xffff);
    toMINIPC.enter = 0x0A;
    encodeSTM32(&toMINIPC, data, sizeof(STM32_data_t));
    VirCom_send(data, sizeof(STM32_data_t));
}

void MINIPC_to_STM32()

{
	Global.Auto.input.shoot_pitch = fromMINIPC.from_minipc_data.shoot_pitch;
	Global.Auto.input.shoot_yaw = fromMINIPC.from_minipc_data.shoot_yaw;
	Global.Auto.input.fire = fromMINIPC.from_minipc_data.fire;
}

int TimeOut = 3; // s
void Auto_control()
{
	
	if (Global.Auto.input.Auto_control_online == 1 || (Get_sys_time_s() - WaitTime) < TimeOut)
	{
		gimbal.yaw.set = ((180.0 / 3.14159265358979323846) * (Global.Auto.input.shoot_yaw));
		gimbal.pitch.set = ((180.0 / 3.14159265358979323846) * (Global.Auto.input.shoot_pitch));
		if (Global.Auto.input.Auto_control_online == 1)
	  WaitTime = Get_sys_time_s();
		Global.Auto.input.Auto_control_online = 0;
		gimbal.speed_mode = 0;
//		if(WaitTime>TimeOut-1.5)
//	     Global.Auto.input.fire=0;
		if ((Get_sys_time_s() - WaitTime) > 1/100.0f)
			Global.Auto.input.fire=0;
		}
	
	
		else
		{
			Scan();
			gimbal.speed_mode = 1;
		}
}
// 扫描模式
void Scan()
{
	//yaw角度增减
	if (TurnOver_yaw == 1 && gimbal.yaw.encoder_degree <= SCAN_RANGE)
	{
		gimbal.yaw_speed= SCAN_YAW_SPEED;
	}
	else if (TurnOver_yaw == 0 && gimbal.yaw.encoder_degree >= -SCAN_RANGE)
	{
		gimbal.yaw_speed= -SCAN_YAW_SPEED;
	}
	
	if (gimbal.yaw.encoder_degree >= SCAN_RANGE)
	{
		TurnOver_yaw = 0;
	}
	if (gimbal.yaw.encoder_degree <= -SCAN_RANGE)
	{
		TurnOver_yaw = 1;
	}

	if (TurnOver_pitch == 1)
	{
		gimbal.pitch_speed= SCAN_PITCH_SPEED;
		if (gimbal.pitch.now >= 5)
			TurnOver_pitch = 0;
	}
	else
	{
		gimbal.pitch_speed= -SCAN_PITCH_SPEED;
		if (gimbal.pitch.now <= -17)
//		if (gimbal.pitch.now <= 0)
			TurnOver_pitch = 1;
	}
}


extern float yaw_original_ecd;
extern float big_yaw_angle;
int Big_yaw_flag = 0; // 0:不转大yaw，1：转大yaw
void control_tran()
{
	switch(flagman)
	{
		case 0:
			yaw_original_ecd = 4000.0f;
		break;
		case 1:
			yaw_original_ecd = 1268.0f;
		break;
		case 2:
			yaw_original_ecd = 6370.0f;
		break;
	}
	
	turnbox();
	
	if (TurnOver_pitch == 1)
	{
		gimbal.pitch_speed= SCAN_PITCH_SPEED;
		if (gimbal.pitch.now >= 5)
			TurnOver_pitch = 0;
	}
	else
	{
		gimbal.pitch_speed= -SCAN_PITCH_SPEED;
		if (gimbal.pitch.now <= -17)
			TurnOver_pitch = 1;
	}
	if(flagdog == 3 && Big_yaw_flag == 0)
	{
		flagdog = 0;
		flagman++;
		if(flagman > 2)
		{
			flagman = 0;
		}
	}
}

void turnbox()
{
	if (TurnOver_yaw == 1 && gimbal.yaw.encoder_degree <= SCAN_RANGE)
	{
		if(flagdog == 3)
		{
			gimbal.big_yaw_speed = SCAN_BIG_YAW_SPEED;
			if(fmod(big_yaw_angle,120.0f) <= EPISION)
			{
				Big_yaw_flag = 0;
				gimbal.big_yaw_speed = 0;
			}
			else
			{
				Big_yaw_flag = 1;
				gimbal.big_yaw_speed = SCAN_BIG_YAW_SPEED;
			}
		}
	    gimbal.yaw_speed= SCAN_YAW_SPEED;
	}
	else if (TurnOver_yaw == 0 && gimbal.yaw.encoder_degree >= -SCAN_RANGE)
	{
		gimbal.yaw_speed= -SCAN_YAW_SPEED;
	}
	if (gimbal.yaw.encoder_degree >= SCAN_RANGE)
	{
		TurnOver_yaw = 0;
		flagdog++;
	}
	if (gimbal.yaw.encoder_degree <= -SCAN_RANGE)
	{
		TurnOver_yaw = 1;
		flagdog++;
	}
}