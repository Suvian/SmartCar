/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * timer : [description]
 *
 * filename : timer.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-23 02:38:55 UTC
 * modified : 2019-04-23 04:14:59 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "timer.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
uint8_t runTime_Flag=0;
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void Speech_TIM_Init(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(SPEECH_TIMxClock, ENABLE);  ///使能TIM3时钟

  TIM_TimeBaseInitStructure.TIM_Period    = arr;  //自动重装载值
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInit(SPEECH_TIMx, &TIM_TimeBaseInitStructure);

  TIM_ITConfig(SPEECH_TIMx, TIM_IT_Update, ENABLE);  //允许定时器3更新中断
  TIM_Cmd(SPEECH_TIMx, ENABLE);                      //使能定时器3

  NVIC_InitStructure.NVIC_IRQChannel = SPEECH_TIMx_IRQn;  //定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03;  //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void SPEECH_TIMx_IRQn_FUN(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)  //溢出中断
  {
    runTime_Flag=1;
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
}

////////////////////////////////// EOF /////////////////////////////////////////
