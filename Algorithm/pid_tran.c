#include "pid_tran.h"
#include <math.h>
void Pid_Init(PIDGroup_t* pid, float Kp, float Ki, float Kd, float IntegralLimit, float LimitOut)
{
	pid -> Kp = Kp;
	pid -> Ki = Ki;
	pid -> Kd = Kd;
	pid -> IntegralLimit = IntegralLimit;
	pid -> LimitOut = LimitOut;
	pid -> Pout = 0.0f;
	pid -> Iout = 0.0f;
	pid -> Dout = 0.0f;
	pid -> TotalOut = 0.0f;

}
void Pid_Enhance(PIDGroup_t* pid,  float DeadZone, float VariableA, float VariableB, 
	                                 float Derivative_LowPassFliter_Coef, float TotalOut_LowPassFliter_Coef)
{
	pid -> DeadZone = DeadZone;
	pid -> CombinationIntegral = 0.0f;
	pid -> VariableA = VariableA;
	pid -> VariableB = VariableB;
	pid -> Derivative_LowPassFliter_Coef = Derivative_LowPassFliter_Coef;
	pid -> TotalOut_LowPassFliter_Coef = TotalOut_LowPassFliter_Coef;
	//异常处理初始化
  pid -> ERRORHandler.ERRORCount = 0;
  pid -> ERRORHandler.ERRORType = PID_ERROR_NONE;
}

//****上述两个pid初始化函数都要使用****//

/**************************************************************************************************************/

//梯形积分
static void Trapezoid_Integral(PIDGroup_t* pid)
{
	pid->CombinationIntegral = pid->Ki * ((pid->Error + pid->LastError) / 2);
	//梯形面积公式，无需多言。
}

//微分先行
static void First_Derivative_On_Measurement(PIDGroup_t* pid)
{
	pid->Dout = (pid->LastMeasure - pid->Measure) * pid->Kd;
	//.原公式为：
	// pid->Dout = pid->Kd * (pid->Error - pid->LastError)
	// pid->Dout =（(pid->Setpoint - pid->LastSetpoint) - (pid->Measure - pid->LastMeasure)） * pid->Kd
	//  pid->Setpoint - pid->LastSetpoint <- 这个会引入异常数值，使得Dout突变

}

// 微分滤波(采集微分时,滤除高频噪声)--"低通滤波"（典型二次滤波法）'低频信号通过，衰减或阻止高频信号'
static void Derivative_Filter(PIDGroup_t* pid)
{
	pid->Dout = pid->Dout * pid->dt / (pid->Derivative_LowPassFliter_Coef + pid->dt) +
	            pid->LastDout * pid->Derivative_LowPassFliter_Coef / (pid->Derivative_LowPassFliter_Coef + pid->dt);
}

//变速积分
static void Changing_Integral_Rate(PIDGroup_t* pid)
{
	if (pid->Error * pid->Iout > 0)
	{
		if (abs(pid->Error) <= pid->VariableB)
			return; //完整积分
		if (abs(pid->Error) <= (pid->VariableA + pid->VariableB))
			//使用线性函数过渡
			pid->CombinationIntegral *= (pid->VariableA - abs(pid->Error) + pid->VariableB) / pid->VariableA;
		else
			pid->CombinationIntegral = 0;//取消积分环节
	}
}

//积分限幅
static void Integral_Limit(PIDGroup_t* pid)
{
	//预期输出，积分输出
	float temp_Output, temp_Iout;
	temp_Iout = pid->Iout + pid->CombinationIntegral;
	temp_Output = pid->Pout + pid->Iout + pid->Dout;
	if (abs(temp_Output) > pid->LimitOut)
	{
		if (pid->Error * pid->Iout > 0)
		{
			//积分仍在增加
			pid->CombinationIntegral = 0;
		}
	}
	//超限幅
	if (temp_Iout > pid->IntegralLimit)
	{
		pid->CombinationIntegral = 0;
		pid->Iout = pid->IntegralLimit;
	}
	if (temp_Iout < -pid->IntegralLimit)
	{
		pid->CombinationIntegral = 0;
		pid->Iout = -pid->IntegralLimit;
	}
}

// 输出滤波--"低通滤波"（典型二次滤波法）'低频信号通过，衰减或阻止高频信号'
static void TotalOut_Filter(PIDGroup_t* pid)
{
	pid->TotalOut= pid->TotalOut * pid->dt / (pid->TotalOut_LowPassFliter_Coef + pid->dt) +
		pid->LastTotalOut * pid->TotalOut_LowPassFliter_Coef / (pid->TotalOut_LowPassFliter_Coef + pid->dt);
}

// 输出限幅
static void TotalOut_Limit(PIDGroup_t* pid)
{
	if (pid->TotalOut > pid->LimitOut)
	{
		pid->TotalOut = pid->LimitOut;
	}
	if (pid->TotalOut < -(pid->LimitOut))
	{
		pid->TotalOut = -(pid->LimitOut);
	}
}
// 电机堵转检测
static void PID_ErrorHandle(PIDGroup_t* pid)
{
	//排除PID输出本身很小的情况
	if (fabsf(pid->TotalOut) < pid->LimitOut * 0.001f || fabsf(pid->Setpoint) < 0.0001f)
		return;

	if ((fabsf(pid->Setpoint - pid->Measure) / fabsf(pid->Setpoint)) > 0.95f)
	{
		//电机堵转计数
		pid->ERRORHandler.ERRORCount++;
	}
	else
	{
		pid->ERRORHandler.ERRORCount = 0;
	}

	if (pid->ERRORHandler.ERRORCount > 500)
	{
		//上述现象持续一段时间则被认定为电机堵转
		pid->ERRORHandler.ERRORType = PID_MOTOR_BLOCKED_ERROR;
	}
}
float Pid_Calculate(PIDGroup_t* pid, float Measure, float Setpoint)
{
	PID_ErrorHandle(pid);
	if (pid->ERRORHandler.ERRORType != PID_ERROR_NONE)
        {
            //电机堵转保护
            pid->TotalOut = 0;
            return 0; 
        }
	pid->dt = DWT_GetDeltaT(&pid->DWT_CNT); // 获取两次pid计算的时间间隔,用于积分和微分
	// 保存上次的测量值和误差,计算当前error
	pid->Measure = Measure;
	pid->Setpoint = Setpoint;
	pid->Error = pid->Setpoint - pid->Measure;

	// 如果在死区外,则计算PID
	if (fabs(pid->Error) > pid->DeadZone)
	{
		// 基本的pid计算,使用位置式
		pid->Pout = pid->Kp * pid->Error;
		pid->CombinationIntegral = pid->Ki * pid->Error * pid->dt;
		pid->Dout = pid->Kd * (pid->Error - pid->LastError) / pid->dt;

		// 梯形积分
		Trapezoid_Integral(pid);
		// 变速积分
		Changing_Integral_Rate(pid);
		// 微分先行
		First_Derivative_On_Measurement(pid);
		// 微分滤波器
		Derivative_Filter(pid);
		// 积分限幅
		Integral_Limit(pid);

		pid->Iout += pid->CombinationIntegral;             // 累加积分
		pid->TotalOut = pid->Pout + pid->Iout + pid->Dout; // 计算输出

		// 输出滤波
		TotalOut_Filter(pid);

		// 输出限幅
		TotalOut_Limit(pid);
	}
	else // 进入死区, 则清空积分和输出
	{
		pid->TotalOut = 0;
		pid->CombinationIntegral = 0;
	}

	// 保存当前数据,用于下次计算
	pid->LastMeasure = pid->Measure;
	pid->LastTotalOut = pid->TotalOut;
	pid->LastDout = pid->Dout;
	pid->LastError = pid->Error;

	return pid->TotalOut;
}
