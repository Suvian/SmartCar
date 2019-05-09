/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * motordriver : [description]
 *
 * filename : motordriver.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-12 02:20:22 UTC
 * modified : 2019-04-16 09:45:52 UTC
\******************************************************************************/

#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "stm32f4xx_conf.h"
#include "sys.h"

#define RIGHT_PULSE GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4)
#define LEFT_PULSE GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5)

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

void Motor_Init(void);
void LED_Init(void);
#endif
////////////////////////////////// EOF /////////////////////////////////////////
