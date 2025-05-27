/*
 * @Author: sethome
 * @Date: 2024-11-15 11:11:53
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-18 20:44:26
 * @FilePath: /25_EE_omni_sentry/Applications/Software/global_status.c
 * @Description: 
 */
#include "global_status.h"
#include "remote_control.h"

struct GlobalStatus_t Global;

/**
 * @description: 全局状态初始化
 * @return {*}
 */
void Global_init(void)
{
    // global
    Global.input.ctl = RC;
    Global.mode = LOCK;
    // gimbal
    Global.input.pitch = 0;
    
    Global.input.yaw = 0;
    // chassis
    Global.input.x = 0;
    Global.input.y = 0;
    Global.input.r = 0;
    // shoot
    Global.input.shoot_status = GLOBAL_DISABLE;
    Global.input.trigger_status = GLOBAL_DISABLE;
}

/**
 * @description: 模式切换
 * @return {*}
 */
void Mode_change()
{
    // 左中右中 底盘跟随
    if (switch_is_mid(RC_L_SW) && switch_is_mid(RC_R_SW))
    {
        Global.mode = FLOW; 
    }
    // 左上右中 小陀螺逆时�?
    else if (switch_is_up(RC_L_SW) && switch_is_mid(RC_R_SW))
    {
        Global.mode = SPIN_L;
    }
    // 左下右中 小陀螺顺时针
    else if (switch_is_down(RC_L_SW) && switch_is_mid(RC_R_SW))
    {
        Global.mode = SPIN_R;
    }
    // 左上右上 原地扫描
    else if (switch_is_up(RC_L_SW) && switch_is_up(RC_R_SW))
    {
        Global.mode = SPIN_SCAN;
    }
    // 左中右上 导航模式
    else if (switch_is_mid(RC_L_SW) && switch_is_up(RC_R_SW))
    {
        Global.mode = NAV;
    }
    // 左下右下 锁死
    else if (switch_is_down(RC_L_SW) && switch_is_down(RC_R_SW))
    {
        Global.mode = LOCK;
    }
    else // 锁死
    {
        Global.mode = LOCK;
    }
}