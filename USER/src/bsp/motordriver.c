/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * motordriver : [description]
 *
 * filename : motordriver.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-12 02:01:08 UTC
 * modified : 2019-04-24 07:53:52 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "motordriver.h"
#include "delay.h"
#include "para_defs.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_syscfg.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
uint16_t RWheelCounter;
uint16_t LWheelCounter;
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void Motor_TIM_Init()
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef       TIM_OCInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);  // TIM10时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);  // TIM11时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);  ///使能TIM14时钟

  // clang-format off
  TIM_TimeBaseStructure.TIM_Prescaler     = PWM_PSC;                   //定时器分频
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseStructure.TIM_Period        = PWM_OVL;  //自动重装载值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);  //初始化定时器10
  TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);  //初始化定时器10

  //初始化TIM10 Channel1 PWM模式
  TIM_OCInitStructure.TIM_OCMode         =TIM_OCMode_PWM1;  //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState    = TIM_OutputState_Enable;  //比较输出使能
  TIM_OCInitStructure.TIM_OCPolarity     =TIM_OCPolarity_Low;  //输出极性:TIM输出比较极性低
  TIM_OC1Init(TIM10, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM10OC1
  TIM_OC1Init(TIM11, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM10OC1

  TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);  //使能TIM10在CCR1上的预装载寄存器
  TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);  //使能TIM10在CCR1上的预装载寄存器
  // clang-format on
  TIM_ARRPreloadConfig(TIM10, ENABLE);  // ARPE使能
  TIM_ARRPreloadConfig(TIM11, ENABLE);  // ARPE使能
  TIM_SetCompare1(TIM10, PWM_OVL * 1/ 6);
  TIM_SetCompare1(TIM11, PWM_OVL * 1/ 6);

  TIM_TimeBaseStructure.TIM_Period    = 2000 - 1;  //自动重装载值
  TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;  // 10KHZ 计数100次为10ms
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);  //初始化TIM14
  TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);  //允许定时器14更新中断

  TIM_Cmd(TIM10, ENABLE);  //使能TIM10
  TIM_Cmd(TIM11, ENABLE);
  TIM_Cmd(TIM14, ENABLE);
}

void Motor_GPIO_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);
  // config PWM IO port
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // right and left direction control io port
  // right PIN0 and PIN1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // config GPIO for velocity pulse counter
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // config gpio for the aviod system
  // PIN12 PIN13 is left
  // clang-format off
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // clang-format on
}

void Motor_NVIC_Init()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;  //定时器14中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03;  //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //使能SYSCFG时钟

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource4);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource5);

  EXTI_InitStructure.EXTI_Line    = EXTI_Line5 | EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;   //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                //中断线使能
  EXTI_Init(&EXTI_InitStructure);                          //配置

  NVIC_InitStructure.NVIC_IRQChannel                   = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                  //配置

  NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                  //配置
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM14, TIM_IT_Update) == SET)  //溢出中断
  {
    RWheelAngularVel = (float) RWheelCounter / (float) 10 / 0.2 * PI;
    LWheelAngularVel = (float) LWheelCounter / (float) 10 / 0.2 * PI;
    RWheelCounter    = 0;
    LWheelCounter    = 0;
  }
  TIM_ClearITPendingBit(TIM14, TIM_IT_Update);  //清除中断标志位
}

void EXTI4_IRQHandler(void)
{
  // delay_ms(10);
  if (!RIGHT_PULSE) {
    RWheelCounter++;
    // printf("%d\r\n", RWheelCounter);
  }
  EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE3上的中断标志位
}

void EXTI9_5_IRQHandler(void)
{
  // delay_ms(10);
  if (!LEFT_PULSE) {
    LWheelCounter++;
    // PFout(9) = ((LWheelCounter % 100 > 0) && (LWheelCounter % 100 < 50)) ? 1 : 0;
    printf("%d\r\n", LWheelCounter);
  }
  EXTI_ClearITPendingBit(EXTI_Line5);
}

void Motor_Init()
{
  Motor_TIM_Init();
  Motor_GPIO_Init();
  Motor_NVIC_Init();
  PFout(0) = 0;
  PFout(1) = 1;
  PFout(2) = 0;
  PFout(3) = 1;
  delay_ms(500);
  // clang-format off
  LWheelAngularVel_HalfDuty = LWheelAngularVel;
  RWheelAngularVel_HalfDuty = RWheelAngularVel;
  CarAngularVel_HalfDuty    = Radius * (LWheelAngularVel + RWheelAngularVel) / WheelDis;
  CarLineVel_HalfDuty       = Radius * (RWheelAngularVel + LWheelAngularVel) / 2;
  // clang-format on
  PFout(0) = 1;
  PFout(1) = 1;
  PFout(2) = 1;
  PFout(3) = 1;
}

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //使能GPIOF时钟

  // GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  // LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;           //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);                   //初始化GPIO

  GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭

}
////////////////////////////////// EOF /////////////////////////////////////////
