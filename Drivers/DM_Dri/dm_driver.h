/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-12-03 21:34:42
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-03 23:15:42
 * @FilePath: /25_EE_omni_sentry/Drivers/DM_Dri/dm_driver.h
 * @Description: 
 */
#ifndef DM_DRIVER_H
#define DM_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "can.h"
#include "struct_typedef.h"

#define MIT_MODE 			0x000
#define POS_MODE			0x100
#define SPEED_MODE		0x200

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -10.0f
#define T_MAX 10.0f


// Define any necessary macros
#define DM_DRIVER_VERSION "1.0.0"

// Declare any necessary types

// 电机回传信息结构体
typedef struct 
{
	int id;
	int state;
	int p_int;
	int v_int;
	int t_int;
	int kp_int;
	int kd_int;
	float pos;
	float vel;
	float tor;
	float Kp;
	float Kd;
	float Tmos;
	float Tcoil;
}motor_fbpara_t;

// 电机参数设置结构体
typedef struct 
{
	int8_t mode;
	float pos_set;
	float vel_set;
	float tor_set;
	float kp_set;
	float kd_set;
}motor_ctrl_t;

typedef struct
{
	int8_t id;
	uint8_t start_flag;
	motor_fbpara_t para;
	motor_ctrl_t ctrl;
	motor_ctrl_t cmd;
}motor_t;




extern motor_t dm4310;
// Declare any necessary functions

void dm4310_init();

float uint_to_float(int x_int, float x_min, float x_max, int bits);
int float_to_uint(float x_float, float x_min, float x_max, int bits);
void dm4310_ctrl_send(CAN_HandleTypeDef* hcan, motor_t *motor);
void dm4310_enable(CAN_HandleTypeDef* hcan, motor_t *motor);
void dm4310_disable(CAN_HandleTypeDef* hcan, motor_t *motor);
void dm4310_set(motor_t *motor);
void dm4310_clear_para(motor_t *motor);
void dm4310_clear_err(CAN_HandleTypeDef* hcan, motor_t *motor);
void dm4310_fbdata(motor_t *motor, uint8_t *rx_data);

void enable_motor_mode(CAN_HandleTypeDef* hcan, uint16_t motor_id, uint16_t mode_id);
void disable_motor_mode(CAN_HandleTypeDef* hcan, uint16_t motor_id, uint16_t mode_id);
void mit_ctrl(CAN_HandleTypeDef* hcan, uint16_t motor_id, float pos, float vel,float kp, float kd, float torq);
void pos_speed_ctrl(CAN_HandleTypeDef* hcan,uint16_t motor_id, float pos, float vel);
void speed_ctrl(CAN_HandleTypeDef* hcan,uint16_t motor_id, float _vel);
void save_pos_zero(CAN_HandleTypeDef* hcan, uint16_t motor_id, uint16_t mode_id);
void clear_err(CAN_HandleTypeDef* hcan, uint16_t motor_id, uint16_t mode_id);
uint8_t canx_bsp_send_data(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t *data, uint32_t len);
#ifdef __cplusplus
}
#endif

#endif // DM_DRIVER_H