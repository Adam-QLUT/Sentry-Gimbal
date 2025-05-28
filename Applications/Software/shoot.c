#include "shoot.h"
#include "stdio.h"
#include "pid.h"
#include "Stm32_time.h"
#include "global_status.h"
#include "referee_handle_pack.h"
#include "math.h"
#include "vofa.h"
PID_t trigger_speed_pid;
PID_t trigger_location_pid;

PID_t shoot1_speed_pid;
PID_t shoot2_speed_pid;
shoot_t shoot;
float back_cnt = 20;	 // 修改这个来更改退弹时间
float back_shoot_gg = 1;
float back_shoot_bb = 10;
float trrigger_rate=4 ;// 弹频 Hz
float trigger_back_cnt = 1;
#define TRIGGER_RPM2HZ 270

void shoot_init()
{
	pid_set(&shoot1_speed_pid, 27, 0, 0, 3500, 0);
	pid_set(&shoot2_speed_pid, 27, 0, 0, 3500, 0);

	pid_set(&trigger_speed_pid, 13, 0.0, 6, 9000, 600);
	pid_set(&trigger_location_pid, 140, 0, 0, 10000, 100);
}

void shoot_update()
{
	decode_as_3508(SHOOT_MOTOR1);
	decode_as_3508(SHOOT_MOTOR2);
	decode_as_2006(TRIGGER_MOTOR);

	shoot.shoot_speed[0] = get_motor_data(SHOOT_MOTOR1).speed_rpm;
	shoot.shoot_speed[1] = get_motor_data(SHOOT_MOTOR2).speed_rpm;

	shoot.trigger_location.now = get_motor_data(TRIGGER_MOTOR).angle_cnt;
	shoot.trigger_speed = get_motor_data(TRIGGER_MOTOR).speed_rpm;
	shoot.trigger_given_current = get_motor_data(TRIGGER_MOTOR).given_current;
}

void shoot_pid_cal(void)
{
	decode_as_3508(SHOOT_MOTOR1);
	decode_as_3508(SHOOT_MOTOR2);
	decode_as_2006(TRIGGER_MOTOR);

	if (Global.input.shoot_status == GLOBAL_ENABLE)
	{
		if (fabs(get_motor_data(SHOOT_MOTOR1).speed_rpm) < 1500 && shoot.speed_level != SHOOT_7000)
		{
			shoot.speed_level = SHOOT_BEGIN; // need rollback to avoid jam
		}
		else
		{
			shoot.speed_level = SHOOT_7000;
		}
	}
	else
	{
		shoot.speed_level = SHOOT_STOP;
	}
	if (Global.input.trigger_status == GLOBAL_ENABLE)
	{
		if (Global.mode != LOCK)
		{
			//HZ:0~8
			if (REFEREE_DATA.Barrel_Heat < 400 && REFEREE_DATA.Barrel_Heat > 300)
			{
				trrigger_rate =(400-REFEREE_DATA.Barrel_Heat)/12.5f ;
			} // 这个需要测试
			else
				trrigger_rate=4;
				shoot.set_trigger_speed = trrigger_rate * TRIGGER_RPM2HZ;
			if (motor_data[17].given_current > 10000)
				trigger_back_cnt = back_cnt; // 更改此值可以改变回退幅度
			if (trigger_back_cnt > 1)
			{
				shoot.set_trigger_speed = -5400.0f;
				trigger_back_cnt--;
			}
		}
		else
			shoot.set_trigger_speed = SHOOT_STOP;
	}
	else
	{
		shoot.set_trigger_speed = SHOOT_STOP;
	}
		
		if(get_motor_data(SHOOT_MOTOR2).speed_rpm < 5000)
				shoot.set_trigger_speed = SHOOT_STOP;

	set_motor(pid_cal(&shoot1_speed_pid, get_motor_data(SHOOT_MOTOR1).speed_rpm, -(float)shoot.speed_level), SHOOT_MOTOR1);
	set_motor(pid_cal(&shoot2_speed_pid, get_motor_data(SHOOT_MOTOR2).speed_rpm, (float)shoot.speed_level), SHOOT_MOTOR2);
	set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.set_trigger_speed), TRIGGER_MOTOR);
	// UploadData_vofa(0,0,0,Global.Auto.input.fire);
}
void shoot_up_up()
{
	
}