/*
 * @Author: sethome
 * @Date: 2024-11-14 19:30:18
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-15 22:45:57
 * @FilePath: /25_EE_omni_sentry/Applications/Software/global_status.h
 * @Description:
 */

#include "stdint.h"

#define __GLOBAL_STATUS_H__
#ifdef __GLOBAL_STATUS_H__

#define GLOBAL_ENABLE 1
#define GLOBAL_DISABLE 0
// 错误码
enum err_e
{
    GIMBAL_ERR = 0,
    CHASSIS_ERR,
    SHOOT_ERR,
    CAP_ERR,
    REMOTE_ERR,
    PC_ERR,
};
enum ctl_e
{
    RC = 0,
    PC,
};
// 适合十几个简单状态的情况
struct GlobalStatus_t
{
    uint8_t err[6]; // 9

    enum mode_e
    {
        LOCK = 0,
        FLOW,
        SPIN_L,
        SPIN_R,
        SPIN_SCAN,
        NAV,
    } mode;
    // super cap
    enum cap_e
    {
        STOP = 0, // 此时电容应在充电
        FULL,     // 全力响应
    } cap;
    //auto control
    struct
    {
        enum auto_mode_e
        {
            NONE,       // 不启用自瞄
            CAR,        // 车
            OUTPOST,    // 前哨站
            LOWTARGET,  // 小符
            HIGHTARGET, // 大符
        } mode;
        struct
        {
            float shoot_yaw;
            float shoot_pitch;
            uint8_t Auto_control_online; //自瞄刷新
            uint8_t fire;      // 是否启用自瞄
            uint8_t target_id; // 目标ID
        } input;
    } Auto;
    //chassis control
    struct
    {
        /* data */
        float x, y, r; // 底盘移动
        float pitch, yaw;
        uint8_t shoot_status;
        uint8_t trigger_status;
        enum ctl_e ctl;
    } input;
};

extern struct GlobalStatus_t Global;
void Global_init(void);
void Mode_change(void);
#endif
// end of file
