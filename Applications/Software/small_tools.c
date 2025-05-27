/*
 * @Author: sethome
 * @Date: 2024-11-15 20:22:41
 * @LastEditors: baoshan daibaoshan2018@163.com
 * @LastEditTime: 2024-11-18 11:00:44
 * @FilePath: /25_EE_omni_sentry/Applications/Software/small_tools.c
 * @Description: 
 */

#include "small_tools.h"


float rad2degree(float a)
{
	return a / PI * 180.0f;
}
float degree2rad(float a)
{
	return a / 180.0f * PI;
}