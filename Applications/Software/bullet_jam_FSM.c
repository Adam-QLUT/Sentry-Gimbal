#include <stdio.h>
#include <time.h>

// 定义状态枚举
typedef enum {
    STATE_NORMAL,
    STATE_SUSPECT,
    STATE_CONFIRMED,
    STATE_HANDLING
} JamState;

// 全局变量
JamState currentState = STATE_NORMAL;
time_t stateStartTime;
float dialMotorTorque;

// 获取拨盘电机当前扭矩电流
float getDialMotorTorque() {
    // 这里可以添加实际获取扭矩电流的代码
    return dialMotorTorque;
}

// 状态机处理函数
void jamStateMachine() {
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, stateStartTime);

    switch (currentState) {
        case STATE_NORMAL:
            if (getDialMotorTorque() > 9.5) {
                currentState = STATE_SUSPECT;
                stateStartTime = currentTime;
                printf("Switch to STATE_SUSPECT\n");
            }
            break;
        case STATE_SUSPECT:
            if (elapsedTime > 0.3) {
                currentState = STATE_CONFIRMED;
                stateStartTime = currentTime;
                printf("Switch to STATE_CONFIRMED\n");
                // 拨盘电机切换至角度闭环，根据当前角度值回拨
                printf("Switch dial motor to angle closed-loop and reverse\n");
            } else if (getDialMotorTorque() <= 9.5) {
                currentState = STATE_NORMAL;
                printf("Switch back to STATE_NORMAL\n");
            }
            break;
        case STATE_CONFIRMED:
            currentState = STATE_HANDLING;
            stateStartTime = currentTime;
            printf("Switch to STATE_HANDLING\n");
            break;
        case STATE_HANDLING:
            if (elapsedTime > 0.2) {
                currentState = STATE_NORMAL;
                printf("Switch back to STATE_NORMAL\n");
            }
            break;
    }
}



    