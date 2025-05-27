#include "vofa.h"
#include "main.h"
#include "stdint.h"
#include "UART_data_transmit.h"
#include "string.h"
#include "stdio.h"


//void UploadData_vofa(float data1,float data2,float data3,float data4)
//{
//	static float temp[4];//float temp[15];
//	temp[0]=data1;
//	temp[1]=data2;
//	temp[2]=data3;
//	temp[3]=data4;
//	memcpy(tempData, (uint8_t *)&temp, sizeof(temp));
//	UART_send_data(UART1_data, tempData,20);

//}

char tempData[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x00,0x00,0x80,0x7F};//前十6个是数据帧

void UploadData_vofa(float data1,float data2,float data3,float data4)
{
    static float temp[4];//float temp[15];
    temp[0]=data1;
    temp[1]=data2;
    temp[2]=data3;
    temp[3]=data4;
    memcpy(tempData, (uint8_t *)&temp, sizeof(temp));
    UART_send_data(UART6_data, (uint8_t *)&tempData,20);

}

