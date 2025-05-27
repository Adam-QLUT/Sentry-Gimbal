/**
 * @file slope.h
 * @author HongXi Wang
 * @author davi
 * @brief 更多的PID
 * @date 2025.5.9
 *
 * @copyright basic_framework-master.HNU-YueLu
 *
 */
//#include "main.h"
#include "stdint.h"
//#include "memory.h"
#include "stdlib.h"
#include "dwt.h"
//#include "arm_math.h"
#include <math.h>
/* PID 报错类型枚举*/
typedef enum 
{
	PID_ERROR_NONE = 0x00U,
	PID_MOTOR_BLOCKED_ERROR = 0x01U
} ErrorType_e;
typedef struct
{
	uint64_t ERRORCount;
	ErrorType_e ERRORType;
} PID_ErrorHandler_t;
typedef struct
{
	float Kp;
	float Kd;
	float Ki;
	float LimitOut;
	float DeadZone;
	float CombinationIntegral;
	float IntegralLimit;
	float Pout;
	float Iout;
	float Dout;
	float LastDout;
	float Setpoint;
	float Measure;
	float LastMeasure;
	float Error;
	float LastError;
	float VariableA;
	float VariableB;
	float dt;
	float Derivative_LowPassFliter_Coef;
	float TotalOut;
	float LastTotalOut;
	float TotalOut_LowPassFliter_Coef;
	uint32_t DWT_CNT;
	PID_ErrorHandler_t ERRORHandler;
}PIDGroup_t;
//PID初始化
void Pid_Init(PIDGroup_t* pid, float Kp,float Ki,float Kd, float IntegralLimit, float LimitOut);
//PID增强参数
void Pid_Enhance(PIDGroup_t* pid, float DeadZone, float VariableA, float VariableB, 
	                                 float Derivative_LowPassFliter_Coef, float TotalOut_LowPassFliter_Coef);
//PID计算
float Pid_Calculate(PIDGroup_t* pid,float Measure,float Setpoint );
