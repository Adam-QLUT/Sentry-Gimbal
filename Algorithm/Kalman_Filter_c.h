/*
 * @Author: baoshan daibaoshan2018@163.com
 * @Date: 2024-12-15 21:20:13
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-15 21:35:23
 * @FilePath: /25_EE_omni_sentry/Algorithm/Kalman_Filter_c.h
 * @Description: 
 */

#ifndef KALMAN_FILTER_C_H
#define KALMAN_FILTER_C_H

#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
//不要删，反正删了会有问题
typedef struct 
{
    float yaw;
    float pitch;
    float roll;
    float unbiased_gyro_x;
    float unbiased_gyro_y;
    float unbiased_gyro_z;
} Attitude_3D_t;

typedef struct {
    float X_last; //上一时刻的最优结果  X(k-|k-1)
    float X_mid;  //当前时刻的预测结果  X(k|k-1)
    float X_now;  //当前时刻的最优结果  X(k|k)
    float P_mid;  //当前时刻预测结果的协方差  P(k|k-1)
    float P_now;  //当前时刻最优结果的协方差  P(k|k)
    float P_last; //上一时刻最优结果的协方差  P(k-1|k-1)
    float kg;     //kalman增益
    float A;      //系统参数
    float B;
    float Q;
    float R;
    float H;
}extKalman_t;
typedef struct
{
  extKalman_t Angle_KF;
  extKalman_t Out_KF;
  float Angle;                      //角度  （坐标系的角度其实就是误差）                     //角加速度
  float Out;//总输出	
}KF_t;

extern KF_t yaw_auto_kf;
extern KF_t pitch_auto_kf;
extern KF_t mouse_x_kf_fliter;
extern KF_t mouse_y_kf_fliter;

void KalmanCreate(extKalman_t *p,float T_Q,float T_R);
float Kalman_Filter(extKalman_t* p,float dat);

void Kalman_Init(void);
float AutoAim_pitch_Algorithm(KF_t *str);//pitch
float AutoAim_Algorithm(KF_t *str,float input);//yaw




#ifdef __cplusplus
}
#endif
#endif
