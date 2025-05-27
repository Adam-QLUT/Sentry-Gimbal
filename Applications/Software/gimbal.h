/*
 * @Author: sethome
 * @Date: 2024-11-15 20:55:13
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-22 15:13:23
 * @FilePath: /25_EE_AGV_sentry/Applications/Software/gimbal.h
 * @Description: Header file for gimbal control
 */

#ifndef GIMBAL_H
#define GIMBAL_H

#include "pid.h"
#include <stdint.h>

// Constants
//#define PITCH_MOTOR CAN_2_6020_5
//#define YAW_MOTOR CAN_2_6020_6
#define PITCH_MOTOR CAN_2_5
#define YAW_MOTOR CAN_2_6
#define PI 3.1415926f
#define SCAN_PITCH_SPEED 4.0F
#define SCAN_YAW_SPEED 1.5F
#define SCAN_BIG_YAW_SPEED 1.5F

#define SCAN_RANGE 60.0f //扫描角度
#define FLOW_SCAN_RANGE 120.0f
#define ENCODER2DEGREE 8191.0/360.0*60 //
//#define yaw_original_ecd 4000.0f//小yaw朝向正前方时编码器的值 
#define YAW_LIMIT_ENCODER_ECD_ANGLE 140.f//ecd角s
// Gimbal status enumeration

typedef enum
{
    ECD = 0,
    IMU,
    SPEED,
    ZERO_FORCE,
} motor_control_e;

typedef enum
{
    AUTO,
    SCAN,
    AUTO_SCAN,
    RC_CONTROL,
    nav,
    lock,
    
} gimbal_status_e;

// Gimbal status structure
typedef struct
{
    gimbal_status_e gimbal_status;
    motor_control_e pitch_status;
    motor_control_e yaw_status;
		motor_control_e big_yaw_status;//?óyaw
    float pitch_speed;
    float pitch_speed_M;
    float yaw_speed;
    float yaw_speed_M;
		float big_yaw_speed;
		float big_yaw_speed_M;
    int speed_mode;
	float last_auto_yaw;
    struct
    {
        float set;
        float now;
        float last;
        float IMU_offset;
        float ECD_offset;
        float stable;
        float set_pitch_speed;
    } pitch; // Pitch data

    struct
    {
        float set;
        float now;
        float last;
        float IMU_offset;
        float ECD_offset;
        float stable;
        float set_yaw_speed;
        float encoder_degree;//degree 
        float encoder_rad;
        float left_limit_angle;
        float right_limit_angle;

    } yaw; // Yaw data
		
		struct
    {
        float set;
        float now;
        float last;
        float IMU_offset;
        float ECD_offset;
        float stable;
        float set_yaw_speed;

    } big_yaw; // big Yaw data
		
		struct
		{
			float imu_yaw;
			float imu_pitch;
			float imu_gyro[3];
		}chassis_board;

} gimbal_status_t;

extern gimbal_status_t gimbal;
extern float yaw_speed;
extern float yaw_auto_angle_cnt;
extern float yaw_error;
extern float m;


// Function declarations
void gimbal_mode_change();
void gimbal_init(void);                  // Initialize gimbal
void gimbal_pid_cal(void);               // Perform PID calculation
void gimbal_update(void);                // Update gimbal data
void gimbal_offset(float pitch_offset, float yaw_offset);
void gimbal_limit(float pitch_up_angle, float pitch_down_angle, float yaw_L_angle, float yaw_R_angle);
void Gimbal_set_yaw_angle(float angle);
void Gimbal_set_pitch_angle(float angle);
void lil_yaw_ecd_limit(PID_t *pid);

void relative_angle_big_yaw_receive(uint8_t data[8]);
void chassis_imu_receive_1(uint8_t data[8]);
void chassis_imu_receive_2(uint8_t data[8]);
float bytes_to_float(uint8_t *bytes);
void scanmode(void);
#endif // GIMBAL_H
