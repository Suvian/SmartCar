/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * esp8266 : [description]
 *
 * filename : esp8266.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 04:00:58 UTC
 * modified : 2019-04-23 03:12:37 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "esp8266.h"
#include "string.h"
#include "para_defs.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
struct STRUCT_USARTx_Fram strEsp8266_Fram_Record;
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
/*************************硬件配置*************************************/
void ESP8266_Init(void)
{
  ESP8266_GPIO_Init();
  ESP8266_USART_Init();
  ESP8266_RST_HIGH_LEVEL();
  ESP8266_CH_DISABLE();
}

void ESP8266_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 配置 CH_PD 引脚*/
  ESP8266_CH_PD_APBxClock_FUN(ESP8266_CH_PD_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = ESP8266_CH_PD_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(ESP8266_CH_PD_PORT, &GPIO_InitStructure);

  /* 配置 RST 引脚*/
  ESP8266_RST_APBxClock_FUN(ESP8266_RST_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = ESP8266_RST_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStructure);
}

void ESP8266_USART_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* config USART clock */
  ESP8266_USART_APBxClock_FUN(ESP8266_USART_CLK, ENABLE);
  ESP8266_USART_GPIO_APBxClock_FUN(ESP8266_USART_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(ESP8266_USART_PORT, ESP8266_USART_TX_AFPIN, GPIO_AF_USART3);
  GPIO_PinAFConfig(ESP8266_USART_PORT, ESP8266_USART_RX_AFPIN, GPIO_AF_USART3);
  /* USART GPIO config */
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = ESP8266_USART_TX_PIN | ESP8266_USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ESP8266_USART_PORT, &GPIO_InitStructure);

  // /* Configure USART Rx as input floating */
  // GPIO_InitStructure.GPIO_Pin  = ESP8266_USART_RX_PIN;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  // GPIO_Init(ESP8266_USART_RX_PORT, &GPIO_InitStructure);

  /* USART mode config */
  USART_InitStructure.USART_BaudRate   = ESP8266_USART_BAUD_RATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;  //无奇偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(ESP8266_USARTx, &USART_InitStructure);

  /* 中断配置 */
  USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, ENABLE);  //使能串口接收中断
  USART_ITConfig(ESP8266_USARTx, USART_IT_IDLE, ENABLE);  //使能串口总线空闲中断

  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                   = ESP8266_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(ESP8266_USARTx, ENABLE);
}

/*
 *the function of interrupter
 */
void ESP8266_USART_INT_FUN(void)
{
  uint8_t ucCh;
  char*   isOK;

  if (USART_GetITStatus(ESP8266_USARTx, USART_IT_RXNE) != RESET) {
    ucCh = USART_ReceiveData(ESP8266_USARTx);

    //预留一个字节写结束符
    if (strEsp8266_Fram_Record.InfBit.FramLength < (RX_BUF_MAX_LEN - 1)) {
      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++] =
        ucCh;
    }
  }

  if (USART_GetITStatus(ESP8266_USARTx, USART_IT_IDLE) == SET)  //数据帧接收完毕
  {
    strEsp8266_Fram_Record.InfBit.FramFinishFlag = 1;
    ucCh           = USART_ReceiveData(ESP8266_USARTx);
    TermInstr_Come = 1;
    if (!( strstr(strEsp8266_Fram_Record.Data_RX_BUF, "terminal_"))) {
      strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
      TermInstr_Come                               = 0;
    }
  }

}
  ////////////////////////////////// EOF /////////////////////////////////////////
