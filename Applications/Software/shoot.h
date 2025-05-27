#ifndef SHOOT_H
#define SHOOT_H

#define SHOOT_MOTOR1 CAN_2_4
#define SHOOT_MOTOR2 CAN_2_2
#define TRIGGER_MOTOR CAN_2_7
#define trriger_delay_max 25.f
#define trriger_back_max 25.f

#include "CAN_Re_Se.h"


enum shoot_speed 
{
	SHOOT_BEGIN = -2000,
    SHOOT_STOP = 0,
    SHOOT_7000 = 6300,
};
typedef struct
{
    /* data */
    enum shoot_speed speed_level;
    float shoot_speed[2]; // 摩擦轮速度
    struct
    {
        /* data */
        float now;
        float set;
        float off_set;
    } trigger_location; // 拨弹电机位置

    float trigger_speed; // 拨弹电机速度
    float set_trigger_speed;
    float trigger_given_current; 		
} shoot_t;


extern fp32 rollback_time;
extern shoot_t shoot;
void shoot_init();
void shoot_update();
void shoot_pid_cal();
void shoot_up_up();


#endif // SHOOT_H
