/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * esp8266 : [description]
 *
 * filename : esp8266.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 04:00:58 UTC
 * modified : 2019-04-23 11:28:54 UTC
\******************************************************************************/

#ifndef __ESP8266_H__
#define __ESP8266_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <stdbool.h>
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//


/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN 1024  //最大接收缓存字节数

extern struct STRUCT_USARTx_Fram  //串口数据帧的处理结构体
{
  char Data_RX_BUF[RX_BUF_MAX_LEN];

  union {
    __IO u16 InfAll;
    struct {
      __IO u16 FramLength : 15;     // 14:0
      __IO u16 FramFinishFlag : 1;  // 15
    } InfBit;
  };

} strEsp8266_Fram_Record;

/******************************** ESP8266 连接引脚定义 ***********************************/
// clang-format off
#define      ESP8266_CH_PD_APBxClock_FUN        RCC_AHB1PeriphClockCmd
#define      ESP8266_CH_PD_CLK                  RCC_AHB1Periph_GPIOA  
#define      ESP8266_CH_PD_PORT                 GPIOA
#define      ESP8266_CH_PD_PIN                  GPIO_Pin_9

#define      ESP8266_RST_APBxClock_FUN          RCC_AHB1PeriphClockCmd
#define      ESP8266_RST_CLK                    RCC_AHB1Periph_GPIOA
#define      ESP8266_RST_PORT                   GPIOA
#define      ESP8266_RST_PIN                    GPIO_Pin_10

#define      ESP8266_USART_BAUD_RATE            115200

#define      ESP8266_USARTx                     USART3
#define      ESP8266_USART_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define      ESP8266_USART_CLK                  RCC_APB1Periph_USART3
#define      ESP8266_USART_GPIO_APBxClock_FUN   RCC_AHB1PeriphClockCmd
#define      ESP8266_USART_GPIO_CLK             RCC_AHB1Periph_GPIOD     
#define      ESP8266_USART_PORT                 GPIOD   
#define      ESP8266_USART_TX_PIN               GPIO_Pin_8
#define      ESP8266_USART_TX_AFPIN             GPIO_PinSource8
#define      ESP8266_USART_RX_PIN               GPIO_Pin_9
#define      ESP8266_USART_RX_AFPIN             GPIO_PinSource9
// unfind define
#define      ESP8266_USART_IRQ                  USART3_IRQn
#define      ESP8266_USART_INT_FUN              USART3_IRQHandler

// #define     ESP8266_Usart( fmt, ... )           USART_printf ( ESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
// #define     PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )    

#define     ESP8266_CH_ENABLE()                 GPIO_SetBits ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN )
#define     ESP8266_CH_DISABLE()                GPIO_ResetBits ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN )

#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )
// clang-format on 

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void ESP8266_Init(void);
void ESP8266_GPIO_Init(void);
void ESP8266_USART_Init(void);
#endif  // __ESP8266_H__
////////////////////////////////// EOF /////////////////////////////////////////
