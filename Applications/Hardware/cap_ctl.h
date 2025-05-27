/*
 * @Author: sethome
 * @Date: 2024-11-14 19:56:46
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-12-22 15:26:18
 * @FilePath: /B_SENTRY_GOTOFINAL (1)/RM_Sentry_7_290630/Applications/Hardware/cap_ctl.h
 * @Description: 
 */
#define __CAP_H__
#ifdef __CAP_H__

#include "stdint.h"

typedef struct // μ?èY×′ì??á11ì?
{
	uint8_t set_max_power; // 最大功率
	uint8_t cache_energy;  // 缓冲电量

	float remain_vol;	   // 剩余电压
	float prediect_energy; // 预测容量 0 - 100%
} cap_t;

extern cap_t cap;

void cap_handle_message(uint8_t data[8]);
void cap_update(void);
int cap_set_power(uint8_t set);
float cap_get_remain_vol(void);
float cap_get_predict_energy(void);
#endif
