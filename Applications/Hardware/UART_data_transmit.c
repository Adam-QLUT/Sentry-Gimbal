/**
 * @file UART_data_transmit.c
 * @author sethome
 * @brief 串口数据发送
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022 sethome
 *
 */
#include "fifo.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "UART_data_transmit.h"
#include "referee_usart_task.h"
// #include "referee_handle_pack.h"
// #include "referee_usart_task.h"
#include "NUC_communication.h"

// DMA控制变量
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;

// 串口控制变量
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

// 将上述串口+DMA整合，并包含缓冲区
transmit_data UART1_data;
transmit_data UART6_data;

// 初始化串口
void uart_init(void)
{
  UART_DMA_rxtx_start(&UART1_data, &huart1, &hdma_usart1_rx, &hdma_usart1_tx);
  UART_DMA_rxtx_start(&UART6_data, &huart6, &hdma_usart6_rx, &hdma_usart6_tx);
}

// ヾ(?ω?`)o温馨提示，使用 DMA + 中断空闲，缓冲区爆了。。就是爆了 固定256字节，暂时没有给你自定义的打算
//  DMA，串口中断启动
void UART_DMA_rxtx_start(transmit_data *data, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_usart_tx)
{
  data->huart = huart;                 // 串口控制变量
  data->hdma_usart_rx = hdma_usart_rx; // DMA接收缓冲
  data->hdma_usart_tx = hdma_usart_tx; // DMA发送缓冲

  HAL_UART_Receive_DMA(data->huart, data->rev_data, 512); // 使能DMA

  __HAL_UART_ENABLE_IT(data->huart, UART_IT_RXNE); // 启动接收中断
  __HAL_UART_ENABLE_IT(data->huart, UART_IT_IDLE); // 启动空闲中断
  __HAL_DMA_ENABLE(hdma_usart_rx);                 // 启动DMA接收
}

// 发送数据（数据别释放了，不然后面收不到）
void UART_send_data(transmit_data uart, uint8_t data[], uint16_t size)
{
	//+++++++++++++++//while(HAL_DMA_GetState(UART6_data.hdma_usart_tx) != HAL_DMA_STATE_READY)	
  HAL_UART_Transmit_DMA(uart.huart, data, size); // 套娃ヾ(?ω?`)o
}

 //请放于UARTx_IRQHandler下，即UART全局中断函数
//=.=当然你想放在HAL_UART_RxCpltCallback也不是不行
void UART_rx_IRQHandler(transmit_data *uart)
{
   uint16_t len; // 得到的数据长度

  if (uart->huart->Instance->SR & UART_FLAG_RXNE) // 接受单字节中断
  {
    // 如果需要对单个字符处理，在此处
  }
  else if (uart->huart->Instance->SR & UART_FLAG_IDLE) // 如果为空闲中断
  {
    HAL_UART_DMAStop(uart->huart); // 失效DMA

    len = 512 - __HAL_DMA_GET_COUNTER(uart->hdma_usart_rx); // 计算获得的字节长度

    // 此处放置处理函数，蟹蟹
    // 数据数组 = uart->rev_data ,长度 = len
    // 自发自收示例： UART_send_data(UART1_data,uart->rev_data,len);

    if (uart->huart == &huart1) // 串口2数据处理
    {
    	if(uart->rev_data[0]==0xAA)
		{
			if(uart->rev_data[1]==19)
			{
				if(uart->rev_data[18]==0x01||uart->rev_data[18]==0x00)
				{
					memcpy((void *)&Navigation_receive_1,&uart->rev_data,19);		
				}
			}
		}
    }
    else if (uart->huart == &huart6) // 串口1数据处理
    {
      // UART_send_data(UART6_data,uart->rev_data,len); // 自发自收
       fifo_s_puts(&referee_fifo, (char *)uart->rev_data, len);
    }

    // 清除数据
    for (int p = 0; p < 512; p++)
      uart->rev_data[p] = 0;
    __HAL_UART_CLEAR_IDLEFLAG(uart->huart);                 // 清除空闲中断标志
    __HAL_UART_CLEAR_PEFLAG(uart->huart);                   // 清除等待标志
    HAL_UART_Receive_DMA(uart->huart, uart->rev_data, 512); // 重新使能DMA缓冲区
  }

  __HAL_UART_CLEAR_PEFLAG(uart->huart); // 清除等待标志
}

// end of file
