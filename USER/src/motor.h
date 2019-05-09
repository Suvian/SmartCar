/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * motor : [description]
 *
 * filename : motor.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-29 06:50:28 UTC
 * modified : 2019-04-24 07:25:24 UTC
\******************************************************************************/

#ifndef __MOTOR_H__
#define __MOTOR_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "para_defs.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_conf.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
#define LeftWheel 0
#define RightWheel 1

#define RIGHT1 PFout(0)
#define RIGHT2 PFout(1)
#define LEFT1 PFout(2)
#define LEFT2 PFout(3)

#define LINFR1 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
#define LINFR2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define RINFR1 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define RINFR2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)

#define Correction 0.9

#define Right_SetDuty(duty) TIM_SetCompare1(TIM10, duty)
#define Left_SetDuty(duty) TIM_SetCompare1(TIM11, duty)

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void GoLeft(void);
void GoRight(void);
void GoAhead(void);
void GoBack(void);
void Break(void);
void Turn(float degree);
void GoStraight(float distance);
void GoPoxy(position Poxy);

void SetSpeed(u16 wheel, u16 speed);

float GetAngularVel(void);
float GetLineVel(void);
void  Avoid(void);
#endif  // __MOTOR_H__
////////////////////////////////// EOF /////////////////////////////////////////
