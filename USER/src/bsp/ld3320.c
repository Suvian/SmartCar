/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * ld3320 : [description]
 *
 * filename : ld3320.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-23 01:23:59 UTC
 * modified : 2019-04-23 05:38:22 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "ld3320.h"
#include "stm32f4xx_conf.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
uint8_t speech_Instr[50] = {0};
uint8_t speechInstrCome  = 0;
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void LD3320_USART_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* config USART clock */
  LD3320_USART_APBxClock_FUN(LD3320_USART_CLK, ENABLE);
  LD3320_USART_GPIO_APBxClock_FUN(LD3320_USART_GPIO_CLK, ENABLE);

  // GPIO_PinAFConfig(LD3320_USART_PORT, LD3320_USART_TX_AFPIN, GPIO_AF_USART3);
  GPIO_PinAFConfig(LD3320_USART_PORT, LD3320_USART_RX_AFPIN, GPIO_AF_USART2);
  /* USART GPIO config */
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = LD3320_USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(LD3320_USART_PORT, &GPIO_InitStructure);

  /* USART mode config */
  USART_InitStructure.USART_BaudRate   = LD3320_USART_BAUD_RATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;  //无奇偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx;
  USART_Init(LD3320_USARTx, &USART_InitStructure);

  /* 中断配置 */
  USART_ITConfig(LD3320_USARTx, USART_IT_RXNE, ENABLE);  //使能串口接收中断
  USART_ITConfig(LD3320_USARTx, USART_IT_IDLE, ENABLE);  //使能串口总线空闲中断

  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                   = LD3320_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(LD3320_USARTx, ENABLE);
}

void LD3320_USART_INT_FUN(void)
{

  if (USART_GetITStatus(LD3320_USARTx, USART_IT_RXNE) != RESET) {
    speech_Instr[0] = USART_ReceiveData(LD3320_USARTx);
  }
  if (USART_GetITStatus(LD3320_USARTx, USART_IT_IDLE) == SET)  //数据帧接收完毕
  {
    USART_ReceiveData(LD3320_USARTx);
  }
  speechInstrCome = 1;
}

void LD3320_Init()
{
  LD3320_USART_Init();
}
////////////////////////////////// EOF /////////////////////////////////////////
