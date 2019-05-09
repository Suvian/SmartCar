/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * calculate_poxy : [description]
 *
 * filename : calculate_poxy.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-21 01:29:31 UTC
 * modified : 2019-04-15 09:34:49 UTC
\******************************************************************************/

#ifndef __CALCULATE_POXY_H__
#define __CALCULATE_POXY_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
// 
#include "stm32f4xx.h"
#include "para_defs.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
// 

////////////////////////////////////////////////////////////////////////////////
// Classes
// 

////////////////////////////////////////////////////////////////////////////////
// Functions
// 
void pos_get(delay_time* delay,position* poxy);
void delay_get(delay_time* delay);
float delay_two_sigs(float* sig, float* sig_vf, uint16_t SamplingNum);

#endif // __CALCULATE_POXY_H__
////////////////////////////////// EOF /////////////////////////////////////////

