/*
 * @Author: sethome
 * @Date: 2024-11-14 22:06:19
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-15 11:28:10
 * @FilePath: /25_EE_omni_sentry/Algorithm/pid.c
 * @Description: 
 */
#include "math.h"
#include "pid.h"

#include <stdio.h>

// // 定义一阶低通滤波器结构体

// // 初始化一阶低通滤波器
// void LowPassFilter_Init(PID_t* filter, float samplingPeriod, float timeConstant) {
//     filter->alpha = samplingPeriod / (timeConstant + samplingPeriod);
//     filter->lastOutput = 0.0f;
// }

// // 执行一阶低通滤波
// float LowPassFilter_Process(PID_t* filter, float input) {
//     float output = filter->alpha * input + (1 - filter->alpha) * filter->lastOutput;
//     filter->lastOutput = output;
//     return output;
// }

void pid_set(PID_t *PidSet,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset)//PID设置
{
  PidSet->p = p_set;   PidSet->i = i_set;   PidSet->d = d_set;
	PidSet->p_out = 0.0f;
	PidSet->i_out = 0.0f;
	PidSet->d_out = 0.0f;
  PidSet->total_out = 0.0f;
  PidSet->set = 0.0f;
  PidSet->lim_out = lim_out_set;   PidSet->lim_i_out = lim_i_outset;//将设置赋值
}

//PID计算
float pid_cal(PID_t *PidGoal,float Now,float Set)//PID??
{
	PidGoal->set = Set;
  PidGoal->last_now = PidGoal->now;
  PidGoal->now = Now;
  PidGoal->err_last = PidGoal->err;
  PidGoal->err = Set - Now;//计算误差
 
  PidGoal->p_out = PidGoal->p * PidGoal->err;
	if(PidGoal->i != 0)
		PidGoal->i_out += PidGoal->i * PidGoal->err;
  PidGoal->d_out = PidGoal->d * (-(PidGoal->now-PidGoal->last_now));//pid运算
  // PidGoal->d_out=LowPassFilter_Process(PidGoal, PidGoal->d_out);
  if(fabs(PidGoal->i_out) > PidGoal->lim_i_out)//防止积分过大
  {
    if(PidGoal->i_out < 0)
      PidGoal->i_out = -PidGoal->lim_i_out;
    else
      PidGoal->i_out = PidGoal->lim_i_out;
  }
	
  PidGoal->total_out = PidGoal->p_out + PidGoal->i_out + PidGoal->d_out;//计算总和输出  

	if(fabs(PidGoal->total_out) > PidGoal->lim_out)//防止总和过大
  {
    if(PidGoal->total_out < 0)
      PidGoal->total_out = -PidGoal->lim_out;
    else
      PidGoal->total_out = PidGoal->lim_out;
  }
	
  return PidGoal->total_out;
}
float calculate_feedforward(PID_t* pid, float setpoint) 
{
    float delta_setpoint = setpoint - pid->prev_setpoint;
    pid->prev_setpoint = setpoint;  // 更新记录
    return pid->Kf * delta_setpoint; // 前馈量 = Kf * 设定值变化率
}

float pid_update(PID_t* pid, float setpoint, float measurement) 
{
    float error = setpoint - measurement;
    float Up = pid->Kp * error + pid->total_out;  // 原PID计算
    float Uf = calculate_feedforward(pid, setpoint); // 前馈计算
    return Up + Uf;  // 合并输出
}//简单前馈最终输出总和
void pid_set_chassis(PID_t *PidSet,float Kf_set,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset)//PID设置
{
  PidSet->p = p_set;   PidSet->i = i_set;   PidSet->d = d_set;
  PidSet->Kf = Kf_set;
  PidSet->lim_out = lim_out_set;   PidSet->lim_i_out = lim_i_outset;//将设置赋值
}//简单前馈，当前仅针对底盘跟随单独使用，待测试
