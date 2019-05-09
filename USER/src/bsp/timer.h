/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * timer : [description]
 *
 * filename : timer.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-23 02:38:55 UTC
 * modified : 2019-04-23 04:14:03 UTC
\******************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
extern uint8_t  runTime_Flag ;

// clang-format off
#define SPEECH_TIMx              TIM3
#define SPEECH_TIMxClock         RCC_APB1Periph_TIM3
#define SPEECH_TIMx_IRQn         TIM3_IRQn
#define SPEECH_TIMx_IRQn_FUN     TIM3_IRQHandler
// clang-format on
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

void Speech_TIM_Init(u16 arr, u16 psc);

#endif  // __TIMER_H__
////////////////////////////////// EOF /////////////////////////////////////////
