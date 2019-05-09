/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * para_defs : [description]
 *
 * filename : para_defs.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-28 08:29:40 UTC
 * modified : 2019-04-23 03:12:15 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "para_defs.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
uint8_t  SamplingOver_Flag = 0;
uint8_t  Voice_Flag        = 0;
uint16_t adc_result[8 * perFifoLen];
float    sig_vf1[2 * VcBuf_Len];
float    sig_vf2[2 * VcBuf_Len];
float    sig2[2 * VcBuf_Len];
float    sig3[2 * VcBuf_Len];
float    sig4[2 * VcBuf_Len];

atap_tag   noise_paramter;
position   pos_dest;
delay_time delay;

uint8_t WriteOverNum    = 0;
uint8_t NowReadFifoNum  = 0;
uint8_t NowWriteFifoNum = 0;

uint8_t FirstFrameFlag = 0;
uint8_t FifoCount      = 0;

// motor paramaters
float RWheelAngularVel = 0;
float LWheelAngularVel = 0;

float LWheelAngularVel_HalfDuty = 0;
float RWheelAngularVel_HalfDuty = 0;
float CarAngularVel_HalfDuty    = 0;
float CarLineVel_HalfDuty       = 0;

uint8_t TermInstr_Come;
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

////////////////////////////////// EOF /////////////////////////////////////////
