/*
 * @Author: sethome
 * @Date: 2024-11-15 20:55:13
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2025-01-09 16:46:44
 * @FilePath: /25_EE_sentry/Applications/Software/gimbal.c
 * @Description:
 */
#include "gimbal.h"
#include "IMU_updata.h"
#include "CAN_Re_Se.h"
#include "global_status.h"
#include "control_setting.h"
#include "UART_data_transmit.h"
#include "NUC_communication.h"
#include "Stm32_time.h"
#include "stm32f4xx_hal.h"
#include "small_tools.h"
#include "IMU_updata.h"
#include "dm_driver.h"
#include "string.h"
#include "slope.h"
#include "pid_tran.h"
#include "vofa.h"
double scan_time_delay;
gimbal_status_t gimbal;
PIDGroup_t gimbal_pid_yaw;

float yaw_original_ecd = 4000.0f;

// ?¨C????¨¨¡ì???¡¯//

Slope slope;

/* ??????????????????????????????pid*/
PID_t pitch_ECD_speed_pid;
PID_t pitch_ECD_location_pid;

PID_t yaw_ECD_speed_pid;
PID_t yaw_ECD_location_pid;
/* ??????????????????????????pid*/
PID_t pitch_IMU_speed_pid;
PID_t pitch_IMU_location_pid;

PID_t yaw_IMU_speed_pid;
PID_t yaw_IMU_pid;
PID_t big_yaw_IMU_pid;
/* ?????????????????????pid*/
PID_t pitch_auto_speed_pid;
PID_t pitch_auto_pid;

PID_t yaw_auto_speed_pid;
PID_t yaw_auto_pid;

/*????????????????????????yaw pid*/
PID_t yaw_Nav_speed_pid;
float yaw_auto_angle_cnt;

float big_yaw_angle;

/* ??????????????????pid*/
PID_t yaw_SCAN_speed_pid;
PID_t yaw_SCAN_pid;
/* ?????????????????????????pid*/
PID_t pitch_location_speed_pid;
PID_t pitch_location_pid;
//????????
float yaw_error;
float m = 0;
float n = 0;
float yaw_cnt = 0;
float random_num = 0;
float yaw_ecd_difference[3] = {0,0,0};
float yaw_update_ecd[3] = {4000.0f,6730.0f,1268.0f};
PIDGroup_t lil_yaw_Speed_pid;
PIDGroup_t lil_yaw_Location_pid;


#include <stdio.h>


void gimbal_init()
{

    memset(&gimbal, 0, sizeof(gimbal_status_t));
    gimbal.yaw_status = IMU;
    gimbal.pitch_status = IMU;

    pid_set(&pitch_ECD_location_pid, 0.0f, 0.0, 0.0f, 30000.0f, 20000.0f);
    pid_set(&pitch_ECD_speed_pid, 0.0f, 0.0f, 0.0f, 8000.0f, 0.01f);

    pid_set(&yaw_ECD_location_pid, 0, 0, 0.0, 25000.0f, 3600.0f);
    pid_set(&yaw_ECD_speed_pid, 0, 0, 0, 8000.0f, 0.0f);

    pid_set(&pitch_IMU_location_pid, 4.7f, 0.012f, 60.0f, 400.0f, 0.0f);      //
    pid_set(&pitch_IMU_speed_pid, 400.0f, 0.54f, 8000.0f, 25000.0f, 3000.0f); //

    pid_set(&yaw_IMU_pid, 4.3f, 0.01f, 100.0f, 100.0f, 1.0f);
    pid_set(&yaw_IMU_speed_pid, 300.0, 0.05f, 15000.0f, 12000.0f, 1500.0f);
    ;
    //???????yaw
    pid_set(&big_yaw_IMU_pid, 4.0f, 0.0, 5.0, 27000.0f, 3600.0f);
    // ????????????pid
    pid_set(&pitch_auto_pid, 500.0f, 0.0, 800.0f, 30000.0f, 20000.0f);
    pid_set(&pitch_auto_speed_pid, 2000.0f, 0.7f, 0.0f, 25000.0f, 5000.0F);

    pid_set(&yaw_auto_pid, 20.0f, 0.05, 0.0, 27000.0f, 100.0f);
    pid_set(&yaw_auto_speed_pid, 2000, 0.1, 0000, 15000.0f, 1500.0f);
    /* ?????????????????????????pid*/
    pid_set(&pitch_location_speed_pid, 0.0f, 0.0, 0.0f, 30000.0f, 20000.0f);
    pid_set(&pitch_location_pid, 0.0f, 0.0f, 0.0f, 8000.0f, 0.0f);

    // ????????????pid (yaw????????????)
    pid_set(&yaw_Nav_speed_pid, 250, 0.1, 0.0, 28000.0f, 0.0f);
    Pid_Init(&lil_yaw_Speed_pid, 0, 0, 0, 0, 0);
    gimbal_offset(0, 0);
    //		Pid_Init(&lil_yaw_Speed_pid,0,0,0,0,0);
    // slope//
    //		Slope_set(&slope,0,0,Slope_First_REAL);
    Pid_Init(&lil_yaw_Location_pid, 0, 0, 0, 0, 0);
    Pid_Init(&lil_yaw_Speed_pid, 0, 0, 0, 0, 0);
    Pid_Enhance(&lil_yaw_Speed_pid, 0, 0, 0, 0, 0);
    Pid_Enhance(&lil_yaw_Location_pid, 0, 0, 0, 0, 0);
}

void gimbal_offset(float pitch_offset, float yaw_offset)
{
    gimbal.pitch.IMU_offset = pitch_offset; //??????imu??????????????????????????????pitch????????
    gimbal.yaw.IMU_offset = yaw_offset;
}

void gimbal_limit(float pitch_up_angle, float pitch_down_angle, float yaw_L_angle, float yaw_R_angle)
{
    // pitch¨¦?????
    if (gimbal.pitch.set <= pitch_up_angle && gimbal.pitch.set < gimbal.pitch.now)
    {
        gimbal.pitch.set = pitch_up_angle;
    }
    else if (gimbal.pitch.set >= pitch_down_angle && gimbal.pitch.set > gimbal.pitch.now)
    {
        gimbal.pitch.set = pitch_down_angle;
    }
    //    if(gimbal.yaw.set-gimbal.yaw.now>180.0f)
    //   gimbal.yaw.set-=360.0f;
    //   else if(gimbal.yaw.set-gimbal.yaw.now<-180.f)
    //   gimbal.yaw.set+=360.0f;
    if ((gimbal.yaw.encoder_degree > YAW_LIMIT_ENCODER_ECD_ANGLE && gimbal.yaw.set > gimbal.yaw.now) || (gimbal.yaw.encoder_degree < -YAW_LIMIT_ENCODER_ECD_ANGLE && gimbal.yaw.set < gimbal.yaw.now))
    {
        gimbal.yaw.set = gimbal.yaw.now;
    }

    // }
    // if(gimbal.yaw.set-gimbal.yaw.now  >  YAW_LIMIT_ENCODER_ECD_ANGLE-gimbal.yaw.encoder_angle||
    //    gimbal.yaw.set-gimbal.yaw.now  < -YAW_LIMIT_ENCODER_ECD_ANGLE-gimbal.yaw.encoder_angle
    // )
    // {
    //       if(gimbal.yaw.encoder_angle>YAW_LIMIT_ENCODER_ECD_ANGLE)

    //       {gimbal.yaw.set=gimbal.yaw.now;
    //       gimbal.yaw.now=gimbal.yaw.encoder_angle;
    //       }
    //       else if(gimbal.yaw.encoder_angle<-YAW_LIMIT_ENCODER_ECD_ANGLE)
    //       {
    //         gimbal.yaw.set=-YAW_LIMIT_ENCODER_ECD_ANGLE;
    //         gimbal.yaw.now=gimbal.yaw.encoder_angle;
    //       }
    // }
}

void lil_yaw_ecd_limit(PID_t *pid)
{
    float limit_set;
    limit_set = pid->set / 360 * 8192;
    if (limit_set > 7000 || limit_set < 850)
        pid->set += 0;
}

void gimbal_update()
{
    decode_as_6020(YAW_MOTOR);
    decode_as_6020(PITCH_MOTOR);
    // yaw
    if (gimbal.yaw_status == ECD)
    {
        gimbal.yaw.now = 0;
        gimbal.yaw_speed = 0;

        gimbal.big_yaw.now = 0;
        gimbal.big_yaw_speed = 0;
    }
    else if (gimbal.yaw_status == IMU)
    {

        //   gimbal.yaw.now = rad2degree(IMU_data.AHRS.yaw);
        gimbal.yaw.now = gimbal.yaw.encoder_degree;

        gimbal.yaw_speed_M = cos(IMU_data.AHRS.pitch) * IMU_data.gyro[2] - sin(IMU_data.AHRS.pitch) * IMU_data.gyro[0];
        gimbal.big_yaw.now = rad2degree(IMU_data.AHRS.yaw_rad_cnt); //???????yaw
        gimbal.yaw.encoder_degree = (motor_data[16].ecd - yaw_original_ecd) / 8192.0f * 360.0f;
        gimbal.yaw.encoder_rad = gimbal.yaw.encoder_degree * 2 * PI / 360.0f;
    }
    // pitch
    if (gimbal.pitch_status == ECD)
    {
        gimbal.pitch.now = 0;
        gimbal.pitch_speed = 0;
    }
    else if (gimbal.pitch_status == IMU)
    {
        gimbal.pitch.now = rad2degree(IMU_data.AHRS.pitch);
        gimbal.pitch_speed_M = IMU_data.gyro[1];
    }
}
/**
 * @description: ???????????????????????????????????????????????????????????????
 * @return {*}
 */
void gimbal_mode_change()
{
    switch (Global.mode)
    {
    case FLOW:
        // FLOW mode operations
        gimbal.gimbal_status = RC_CONTROL;
         gimbal.pitch.set -= Global.input.pitch;
        //        gimbal.yaw.set -= Global.input.yaw;
        // gimbal.yaw.set -= 100*Global.input.yaw;
        gimbal.big_yaw.set -= Global.input.yaw; //???????yaw
        break;
    case SPIN_L:
        // SPIN_L mode operations
        gimbal.gimbal_status = RC_CONTROL;
        gimbal.pitch.set -= Global.input.pitch;
        //        gimbal.yaw.set -= Global.input.yaw;
        gimbal.yaw.set = 0;
        gimbal.big_yaw.set -= Global.input.yaw;
        break;
    case SPIN_R:
        // SPIN_R mode operations
        gimbal.gimbal_status = RC_CONTROL;
        gimbal.pitch.set -= Global.input.pitch;
        //        gimbal.yaw.set -= Global.input.yaw;
        gimbal.yaw.set = 0;
        gimbal.big_yaw.set -= Global.input.yaw;
        break;
    case NAV:
        // NAV mode operations
        if (Navigation_receive_1.x_speed != 0 && Navigation_receive_1.y_speed != 0 && Navigation_receive_1.header != 0 && Navigation_receive_1.yaw_speed != 0) // ??¡è?¨C-??????|?????????¡§?¡¥?¨¨?a
        {
            gimbal.gimbal_status = nav;
        } 
        else
        {
            gimbal.gimbal_status = AUTO_SCAN;
            Auto_control();
        }
        break;
    case SPIN_SCAN:
        // SPIN_SCAN mode operations

        gimbal.gimbal_status = AUTO_SCAN;
        gimbal.pitch.set -= Global.input.pitch;
        //        gimbal.yaw.set -= Global.input.yaw;
        gimbal.yaw.set -= 30 * Global.input.yaw;
        Auto_control();

        break;
    case LOCK:
        // LOCK mode operations
        gimbal.gimbal_status = lock; // ????????????????????AUTO??????????????????????????????
        set_motor(0, YAW_MOTOR);
        set_motor(0, PITCH_MOTOR);
        break;
    default:
        // Default to LOCK mode
        gimbal.gimbal_status = lock;
        set_motor(0, YAW_MOTOR);
        set_motor(0, PITCH_MOTOR);
        break;
    }
}

/**
 * @description: ??????????????????????????????????????????????
 * @return {*}
 */
float t = 0;
void gimbal_pid_cal(void)
{
    gimbal_mode_change();
    gimbal_limit(-27, 19, 0, 0); // pitch_up_angle, pitch_down_angle, yaw_L_angle, yaw_R_angle
    // pitch
    if (gimbal.pitch_status == IMU)
    {
        if (gimbal.gimbal_status == RC_CONTROL)
        {
            gimbal.pitch_speed = pid_cal(&pitch_IMU_location_pid, gimbal.pitch.now, gimbal.pitch.set);
            set_motor(pid_cal(&pitch_IMU_speed_pid, gimbal.pitch_speed_M, gimbal.pitch_speed), PITCH_MOTOR);
            //            yaw_auto_angle_cnt = -gimbal.pitch.now;
        }
        else if (gimbal.gimbal_status == AUTO_SCAN)
        {
            if (gimbal.speed_mode == 1)
            {
                set_motor(pid_cal(&pitch_auto_speed_pid, gimbal.pitch_speed_M, gimbal.pitch_speed), PITCH_MOTOR);
            }
            else if (gimbal.speed_mode == 0)
            {
                gimbal.pitch_speed = pid_cal(&pitch_IMU_location_pid, gimbal.pitch.now, gimbal.pitch.set);
                // gimbal.pitch_speed=t;
                set_motor(pid_cal(&pitch_IMU_speed_pid, gimbal.pitch_speed_M, gimbal.pitch_speed), PITCH_MOTOR); //
            }
        }
    }
    else if (gimbal.pitch_status == ECD)
    {
        if (gimbal.gimbal_status == RC_CONTROL)
        {
        }
        else if (gimbal.gimbal_status == AUTO_SCAN)
        {
        }
    }
    else
        set_motor(0, PITCH_MOTOR);
    // yaw
    if (gimbal.yaw_status == IMU)
    {
        if (gimbal.gimbal_status == RC_CONTROL)
        {
         
            gimbal.yaw_speed = pid_cal(&yaw_IMU_pid, gimbal.yaw.encoder_degree, gimbal.yaw.set);
            //
            set_motor(pid_cal(&yaw_IMU_speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed), YAW_MOTOR); // pid_cal(&yaw_IMU_speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed)
        }

        else if (gimbal.gimbal_status == AUTO_SCAN)
        {

            //  gimbal.yaw_speed = Pid_Calculate(&lil_yaw_Location_pid,gimbal.yaw.now, gimbal.yaw.set);
            //  set_motor(Pid_Calculate(&lil_yaw_Speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed), YAW_MOTOR);
            if (gimbal.speed_mode == 1)
            {
                set_motor(pid_cal(&yaw_auto_speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed), YAW_MOTOR);
            }
            else if (gimbal.speed_mode == 0)
            {
                gimbal.yaw_speed = pid_cal(&yaw_IMU_pid, gimbal.yaw.encoder_degree, gimbal.yaw.set);
                set_motor(pid_cal(&yaw_IMU_speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed), YAW_MOTOR);
            }
        }
        else if (gimbal.gimbal_status == nav)
        {
            gimbal.yaw_speed = pid_cal(&yaw_IMU_pid, gimbal.yaw.now, gimbal.yaw.set);

            set_motor(pid_cal(&yaw_IMU_speed_pid, gimbal.yaw_speed_M, gimbal.yaw_speed), YAW_MOTOR);
        }
    }
    else if (gimbal.yaw_status == ECD)
    {
        if (gimbal.gimbal_status == RC_CONTROL)
        {
        }
        else if (gimbal.gimbal_status == AUTO_SCAN)
        {
        }
    }
    else
        set_motor(0, YAW_MOTOR);
    // UploadData_vofa(gimbal.pitch.set,gimbal.pitch.now,pitch_IMU_speed_pid.d_out,pitch_IMU_speed_pid.total_out);
}
// ????????
void Gimbal_set_yaw_angle(float angle)
{

    gimbal.yaw.set = (angle);
}

void Gimbal_set_pitch_angle(float angle)
{
    gimbal.pitch.set = (-angle);
}

void relative_angle_big_yaw_receive(uint8_t data[8])
{
    big_yaw_angle = bytes_to_float(&data[0]); //????:??
    gimbal.chassis_board.imu_yaw = bytes_to_float(&data[4]);
}
void chassis_imu_receive_1(uint8_t data[8])
{
    gimbal.chassis_board.imu_pitch = bytes_to_float(&data[0]);
    gimbal.chassis_board.imu_gyro[0] = bytes_to_float(&data[4]);
}
void chassis_imu_receive_2(uint8_t data[8])
{
    gimbal.chassis_board.imu_gyro[1] = bytes_to_float(&data[0]);
    gimbal.chassis_board.imu_gyro[2] = bytes_to_float(&data[4]);
}
// ????????????????????????????????????float
float bytes_to_float(uint8_t *bytes)
{
    uint32_t temp = 0;
    temp |= (bytes[0] << 0);  // ??????????????????
    temp |= (bytes[1] << 8);  // ???????????????
    temp |= (bytes[2] << 16); // ???????????????
    temp |= (bytes[3] << 24); // ??????????????????
    return *(float *)&temp;   // ??uint32_t???????????????????float????
}

uint8_t pitch_dir = 1, yaw_dir = 1;
