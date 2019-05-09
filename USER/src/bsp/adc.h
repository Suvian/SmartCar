/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * adc : [description]
 *
 * filename : adc.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-21 09:04:33 UTC
 * modified : 2019-04-09 06:04:48 UTC
\******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
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
void     ADC_Common_Init(void);
void     ADC_Vad_Init(void);
void     ADC_Voice_Init(void);
void     ADC_stop(void);
void     ADC_start(void);
uint32_t ADC_GetDMA2ITStatus(uint32_t DMA_FLAG);
// uint32_t GetCR(void);
#endif  // __ADC_H__
////////////////////////////////// EOF /////////////////////////////////////////
