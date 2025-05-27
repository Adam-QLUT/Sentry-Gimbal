/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-11-14 22:06:27
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-05 16:14:28
 * @FilePath: /25_EE_omni_sentry/Algorithm/pid.h
 * @Description: 
 * /
*/
#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif
typedef struct//PID控制数据结构体
{
  float p,i,d;//比例，积分，微分
  float set,now,last_now,err,err_last;//设置值，误差值，上次的误差值
  float p_out,i_out,d_out,total_out;//比例，积分，微分的输出和总输出
  float lim_i_out,lim_out;//输出积分限制，总输出限制
  float Kp, Ki, Kd;      // PID参数 
  float Kf;              // 前馈增益（新增）
  float prev_setpoint;   // 上次设定值（新增）
  // float alpha;  // 滤波系数
  // float lastOutput;  // 上一次的输出
} PID_t;

extern PID_t pitch_speed_pid;
extern PID_t pitch_location_pid;

extern PID_t yaw_speed_pid;
extern PID_t yaw_location_pid;

extern PID_t trigger_speed_pid;
extern PID_t trigger_location_pid;

extern PID_t shoot1_speed_pid;
extern PID_t shoot2_speed_pid;



void pid_set(PID_t *PidSet,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset);//PID设置
float pid_cal(PID_t *PidGoal,float Now,float Set);//PID计算
float calculate_feedforward(PID_t* pid, float setpoint);
float pid_update(PID_t* pid, float setpoint, float measurement);
void pid_set_chassis(PID_t *PidSet,float Kf_set,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset);//PID设置
#ifdef __cplusplus
}
#endif

#endif // PID_H
