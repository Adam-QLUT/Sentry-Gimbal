/*
 * @Author: sethome
 * @Date: 2024-11-14 19:28:04
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-27 16:39:44
 * @FilePath: /25_EE_omni_sentry/Applications/Software/chassis_move.h
 * @Description: 
 */
#ifndef CHASSIS_MOVE_H
#define CHASSIS_MOVE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "struct_typedef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
//omni

//#define chassis_FR CAN_2_1
//#define chassis_FL CAN_2_2
//#define chassis_BL CAN_2_3
//#define chassis_BR CAN_2_4
//AGV
//#define chassis_FL_speed CAN_1_1
//#define chassis_BL_speed CAN_1_2
//#define chassis_BR_speed CAN_1_3
//#define chassis_FR_speed CAN_1_4

//#define chassis_FL_steer CAN_2_1
//#define chassis_BL_steer CAN_2_2
//#define chassis_BR_steer CAN_2_3
//#define chassis_FR_steer CAN_2_4

struct chassis_status
{
	struct
	{
		float x, y, r;
		float now_x, now_y, now_r;
		float last_x, last_y, last_r;
		float max_x, max_y, max_r; // m/s
	} speed;
    float wheel_rpm[4]; // wheel speed
	int16_t wheel_current[4]; // PID output
	uint8_t is_open_cap;
	float current_power_draw; // 新增：当前底盘实际功耗估算
  float power_limit_percentage; // 新增：当前功率限制百分比（0-1.0）
};
extern struct chassis_status chassis;
extern float lastSteeringAnglearget[4]; 
extern float steeringAngleTarget[4];
// Function 
void get_global_chassis_input();
//omni
void chassis_move_init(void);
void chassis_moto_speed_calc(void);
//AGV
void AGV_chassis_init();
void AGV_chassis_update();
void Chassis_steeringAng_calc();
void AGV_chassis_calc();

//void Send_to_Chassis();	//发送底盘速度数据
void Send_to_Chassis_1();
void Send_to_Chassis_2();
void Send_to_Chassis_3();

float generate_random_float(float min, float max);
float random_anti_vision_r_s(float min, float max);

void float_to_bytes(float f, uint8_t *bytes);

#ifdef __cplusplus
}
#endif

#endif // CHASSIS_MOVE_H