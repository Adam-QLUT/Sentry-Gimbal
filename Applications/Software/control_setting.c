/*
 * @Author: sethome
 * @Date: 2024-11-15 11:06:52
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-22 15:23:30
 * @FilePath: /25_EE_AGV_sentry/Applications/Software/control_setting.c
 * @Description:
 */
#include "control_setting.h"
#include "CAN_Re_Se.h"
#include "shoot.h"
#include "global_status.h"
#include "remote_control.h"
#include "chassis_move.h"
#include <math.h>
#include "gimbal.h"
extern iii=0;
extern ccc=0;
/**
 * @description: 遥控器控制
 * @return {*}
 */
void remote_control_task()
{
    // chassis input
    Global.input.x = -RC_data.rc.ch[0] / 110.0f;
    Global.input.y = -RC_data.rc.ch[1] / 80.0f;

    // gimbal input
    Global.input.yaw = RC_data.rc.ch[2] / 167500.0f;
    Global.input.pitch = RC_data.rc.ch[3] / 11000.0f;



    if ((RC_data.rc.ch[4] > 600 ||( Global.Auto.input.fire)) && Global.mode != LOCK)
    {
        if (fabs(get_motor_data(SHOOT_MOTOR2).speed_rpm) > 5800) // 摩擦轮速度的判断
        {
            Global.input.trigger_status = GLOBAL_ENABLE;
        }
    }
    else
    {
        Global.input.trigger_status = GLOBAL_DISABLE;
    }

    if (RC_data.rc.ch[4] > 300 || gimbal.gimbal_status == AUTO_SCAN)
    {
        Global.input.shoot_status = GLOBAL_ENABLE;
    }
    else
    {
        Global.input.shoot_status = GLOBAL_DISABLE;
    }
}
