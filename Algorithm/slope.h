/**
 * @file slope.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief 斜坡函数, 用于速度规划等
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-06-03 1.1 规划引入优先级方式
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

#ifndef __SLOPE_H
#define __SLOPE_H

/* Includes ------------------------------------------------------------------*/

#include "stdint.h"
#include "struct_typedef.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 规划优先类型, 分为目标值优先和真实值优先
  * 真实值优先, 即当前真实值夹在当前规划值和目标值之间, 当前规划值转为当前真实值
 * 目标值优先, 即硬规划
 *
 */
typedef enum {
    Slope_First_REAL = 0,
    Slope_First_TARGET
} Enum_Slope_First;

/**
 * @brief Reusable, 斜坡函数本体
 *
 */
typedef struct {
    // 输出值
    float Out;

    // 规划优先类型
    Enum_Slope_First Slope_First;
    // 当前规划值
    float Now_Planning;
    // 当前真实值
    float Now_Real;

    // 绝对值增量, 一次计算周期改变值
    float Increase_Value;
    // 绝对值减量, 一次计算周期改变值
    float Decrease_Value;
    // 目标值
    float Target;
} Slope;

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 初始化斜坡函数
 *
 * @param slope 斜坡函数结构体指针
 * @param Increase_Value 绝对值增量
 * @param Decrease_Value 绝对值减量
 * @param Slope_First 规划优先类型
 */
void Slope_set(Slope *slope, float Increase_Value, float Decrease_Value, Enum_Slope_First Slope_First);

/**
 * @brief 计算函数
 *
 * @param slope 斜坡函数结构体指针
 */
float Slope_Cal(Slope *slope,float Now_Real,float Target);


float Math_Abs(float x);

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/