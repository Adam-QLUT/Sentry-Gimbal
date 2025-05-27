/**
 * libAHRS.a
 * @file IMU_updata.c
 * @author sethome
 * @brief IMU updata info
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#define PI 3.1415926f

#include "pid.h"
#include "IMU_updata.h"
#include "BMI088Middleware.h"
#include "BMI088driver.h"
#include "stdint.h"
#include "ist8310driver.h"
#include "fdacoefs.h"

#include "MahonyAHRS.h"
#include "madgwick.h"
#include "AHRS.h"
#include "bsxlite_interface.h"

#include "tim.h"
#include "iwdg.h"
#include "Stm32_time.h"

// made by sethome
// 使用TIM14定频更新

// 陀螺仪温度PID
PID_t IMU_tempure_pid;

//加速度计低通滤波
static fp32 accel_fliter_1[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_2[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_3[3] = {0.0f, 0.0f, 0.0f};
static const fp32 fliter_num[3] = {1.929454039488895f, -0.93178349823448126f, 0.002329458745586203f};
static uint8_t first_temperate;


unsigned  long ulTdleCycleCount = 0UL;	

// 定时器变量
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data; // IMU数据结构体


// 内部调用

void IMU_heat_set(uint16_t ccr); // 加热电阻PWM占空比
void shiftArray(float arr[], int size) ;

static void imu_temp_control(fp32 temp);

// 四元数转为欧拉角 非调用lib版本，勿删
 void Get_angle(fp32 q[4], fp32 *yaw, fp32 *pitch, fp32 *roll)
 {
	*yaw = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 2.0f * (q[0] * q[0] + q[1] * q[1]) - 1.0f);
	*pitch = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
	*roll = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f);
 }

// 初始化
void IMU_init()
{
	ist8310_init(); // 磁力计
	BMI088_init();	// 陀螺仪
	
	IMU_data.Mahony.q[0] = 1.0f;
	IMU_data.Mahony.q[1] = 0.0f;
	IMU_data.Mahony.q[2] = 0.0f;
	IMU_data.Mahony.q[3] = 0.0f;

	IMU_data.madgwick.q[0] = 1.0f;
	IMU_data.madgwick.q[1] = 0.0f;
	IMU_data.madgwick.q[2] = 0.0f;
	IMU_data.madgwick.q[3] = 0.0f;


	AHRS_init(IMU_data.AHRS.q, IMU_data.accel, IMU_data.mag); // AHRS滤波参数
	
	pid_set(&IMU_tempure_pid, 1600, 0.2, 0, 4500, 4400); 
	//后续可能会更改
	HAL_TIM_Base_Start(&htim10);						 // 加热电阻PWM
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);			

	HAL_IWDG_Init(&hiwdg); // 看门狗
	HAL_TIM_Base_Start_IT(&htim13); // 使能更新中断，1000HZ
	HAL_TIM_Base_Start_IT(&htim14); // 使能更新中断，200HZ
	

	// 校准加速度（未完成）和角速度计
	for (int i = 0; i < 1000;)
	{
		float x_ = IMU_data.gyro[0];
		float y_ = IMU_data.gyro[1];
		float z_ = IMU_data.gyro[2];
		if (fabs(x_) > 1.0f && fabs(y_) > 1.0f && fabs(z_) > 1.0f)
			continue;
		IMU_data.calibration[0] += x_ / 1000.0f;
		IMU_data.calibration[1] += y_ / 1000.0f;
		IMU_data.calibration[2] += z_ / 1000.0f;
		i++;
		HAL_Delay(1);
	}
}

void process_IMU_data()
{
	// 计算总圈数
	if (IMU_data.AHRS.last_yaw > 3.0f && IMU_data.AHRS.yaw < -3.0f)
		IMU_data.AHRS.yaw_rad_cnt += ((PI - IMU_data.AHRS.last_yaw) + (IMU_data.AHRS.yaw + PI));
	else if (IMU_data.AHRS.last_yaw < -3.0f && IMU_data.AHRS.yaw > 3.0f)
		IMU_data.AHRS.yaw_rad_cnt -= ((PI + IMU_data.AHRS.last_yaw) + (PI - IMU_data.AHRS.yaw));
	else
		IMU_data.AHRS.yaw_rad_cnt += (IMU_data.AHRS.yaw - IMU_data.AHRS.last_yaw);
}
// IMU更新函数
void IMU_updata() // 1000HZ
{

	HAL_IWDG_Refresh(&hiwdg);
	// 读取陀螺仪和地磁计信息
	BMI088_read(IMU_data.gyro, IMU_data.accel, &IMU_data.temp);
	//零漂移补偿
	
//		IMU_data.gyro[0] -= IMU_data.calibration[0];
//	  IMU_data.gyro[1] -= IMU_data.calibration[1];
//	  IMU_data.gyro[2] -= IMU_data.calibration[2];
		IMU_data.gyro[0] -= 0.0;
	  IMU_data.gyro[1] -= 0.0;
	  IMU_data.gyro[2] += 0.0;
	

////////	// 加热器PID计算
////////	imu_temp_control(IMU_data.temp);

	 //加速度计低通滤波
     //accel low-pass filter
	accel_fliter_1[0] = accel_fliter_2[0];
	accel_fliter_2[0] = accel_fliter_3[0];

	accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + IMU_data.accel[0] * fliter_num[2];

	accel_fliter_1[1] = accel_fliter_2[1];
	accel_fliter_2[1] = accel_fliter_3[1];

	accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] +IMU_data.accel[1] * fliter_num[2];

	accel_fliter_1[2] = accel_fliter_2[2];
	accel_fliter_2[2] = accel_fliter_3[2];

	accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + IMU_data.accel[2] * fliter_num[2];

	// AHRS.lib
	// 如果失败，返回0
	static uint32_t last_time = 0;
	IMU_data.AHRS.err_code = AHRS_update(IMU_data.AHRS.q, (float)(Get_sys_time_ms() - last_time) * 0.001,
										 IMU_data.gyro,
										  accel_fliter_3,
										 IMU_data.mag);
	last_time = Get_sys_time_ms();
	get_angle(IMU_data.AHRS.q, &IMU_data.AHRS.yaw, &IMU_data.AHRS.pitch, &IMU_data.AHRS.roll);
	
	
	process_IMU_data();
	IMU_data.AHRS.last_yaw = IMU_data.AHRS.yaw;

}


void MagUpdate() // 更新地磁计
{
	ist8310_read_mag(IMU_data.mag);
}
void MagZero() // 清零地磁计
{
	IMU_data.mag[0] = 0;
	IMU_data.mag[1] = 0;
	IMU_data.mag[2] = 0;
		
}
// 设定加热的PWM占空比
void IMU_heat_set(uint16_t ccr)
{
	if(ccr < 0)
	{
		ccr = 0;	
	}
	__HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, ccr); // HAL库PWM的CCR设定
													   // TIM10->CCR1 = (pwm);
}

/**
  * @brief          控制bmi088的温度
  * @param[in]      temp:bmi088的温度
  * @retval         none
  */
static void imu_temp_control(fp32 temp)
{
    uint16_t tempPWM;
    static uint8_t temp_constant_time = 0;
    if (first_temperate)
    {
        pid_cal(&IMU_tempure_pid, temp, 45);
		
        if (IMU_tempure_pid.total_out < 0.0f)
        {
            IMU_tempure_pid.total_out  = 0.0f;
        }
        tempPWM = (uint16_t)IMU_tempure_pid.total_out ;
		IMU_heat_set(tempPWM);
    }
    else
    {
        //在没有达到设置的温度，一直最大功率加热
        //in beginning, max power
        if (temp > 45)
        {
            temp_constant_time++;
            if (temp_constant_time > 200)
            {
                //达到设置温度，将积分项设置为一半最大功率，加速收敛
                //
                first_temperate = 1;
                IMU_tempure_pid.i_out = 5000 / 2.0f;
            }
        }

        IMU_heat_set(5000 - 1);
    }
}
void shiftArray(float arr[], int size) 
{
    for (int i = size - 1; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
}

void imu_cail_program(void)//陀螺仪校准程序，放在闲置任务里面，需要的时候校准一次算出平均数值加上即可
{
	
		//不好用，建议直接去imu初始化里面去读那个校准量。
		IMU_data.calibration[0]=0;
		IMU_data.calibration[1]=0;
		IMU_data.calibration[2]=0;
	for (int i = 0; i < 1000;)
		{
			float x_ = IMU_data.gyro[0];
			float y_ = IMU_data.gyro[1];
			float z_ = IMU_data.gyro[2];
			IMU_data.calibration[0] += x_ / 1000.0f;
			IMU_data.calibration[1] += y_ / 1000.0f;
			IMU_data.calibration[2] += z_ / 1000.0f;
			i++;
		}
		if(IMU_data.calibration[0]!=0&&IMU_data.calibration[1]!=0&&IMU_data.calibration[2]!=0)
		{
		IMU_data.cali_end[0] =IMU_data.calibration[0];
		IMU_data.cali_end[1] =IMU_data.calibration[1];
		IMU_data.cali_end[2] =IMU_data.calibration[2];
		}


}
// end of file
