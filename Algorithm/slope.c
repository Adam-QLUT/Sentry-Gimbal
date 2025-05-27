#include "slope.h"



// 求绝对值的函数
float Math_Abs(float x)
{
    return ((x > 0) ? x : -x);
}




/**
 * @brief 初始化斜坡函数
 *
 * @param slope 斜坡函数结构体指针
 * @param Increase_Value 绝对值增量
 * @param Decrease_Value 绝对值减量
 * @param Slope_First 规划优先类型
 */
void Slope_set(Slope *slope, float Increase_Value, float Decrease_Value, Enum_Slope_First Slope_First) {
    slope->Out = 0.0f;
    slope->Slope_First = Slope_First;
    slope->Now_Planning = 0.0f;
    slope->Now_Real = 0.0f;
    slope->Increase_Value = Increase_Value;
    slope->Decrease_Value = Decrease_Value;
    slope->Target = 0.0f;
}


/**
 * @brief 斜坡函数调整值, 计算周期取决于调用者
 *
 * @param slope 斜坡函数结构体指针
 */
float Slope_Cal(Slope *slope,float Now_Real,float Target) {
		slope->Now_Real = Now_Real;
	  slope->Target = Target;
    // 规划为当前真实值优先的额外逻辑
    if (slope->Slope_First == Slope_First_REAL) {
        if ((slope->Target >= slope->Now_Real && slope->Now_Real >= slope->Now_Planning) ||
            (slope->Target <= slope->Now_Real && slope->Now_Real <= slope->Now_Planning)) {
            slope->Out = slope->Now_Real;
        }
    }

		if(slope->Slope_First == Slope_First_TARGET)
		{
			if (slope->Now_Planning > 0.0f)
				{
        if (slope->Target > slope->Now_Planning) 
					{
            // 正值加速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Increase_Value)
							{
                slope->Out += slope->Increase_Value;
							} 
						else
							{
								slope->Out = slope->Target;
							}
					}
				else if (slope->Target < slope->Now_Planning) 
					{
            // 正值减速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Decrease_Value) 
							{
                slope->Out -= slope->Decrease_Value;
              } 
						else 
							{
                slope->Out = slope->Target;
              }
          }
			  } 
		  else if (slope->Now_Planning < 0.0f) 
				{
        if (slope->Target < slope->Now_Planning)
					{
            // 负值加速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Increase_Value) 
							{
                slope->Out -= slope->Increase_Value;
              } 
						else 
							{
                slope->Out = slope->Target;
              }
          } 
				else if (slope->Target > slope->Now_Planning)
					{
            // 负值减速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Decrease_Value) 
							{
                slope->Out += slope->Decrease_Value;
              } 
						else 
							{
                slope->Out = slope->Target;
              }
          }
       } 
			else 
				{
        if (slope->Target > slope->Now_Planning)
					{
            // 0值正加速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Increase_Value)
							{
                slope->Out += slope->Increase_Value;
              } 
						else 
							{
                slope->Out = slope->Target;
              }
          }
				else if (slope->Target < slope->Now_Planning)
					{
            // 0值负加速
            if (Math_Abs(slope->Now_Planning - slope->Target) > slope->Increase_Value) 
							{
                slope->Out -= slope->Increase_Value;
              }
						else 
							{
                slope->Out = slope->Target;
              }
           }
        }
		}
    
    // 善后工作
    slope->Now_Planning = slope->Out;
		return slope->Now_Planning;
} 
