/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * para_defs : [description]
 *
 * filename : para_defs.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-28 08:29:20 UTC
 * modified : 2019-04-23 03:12:03 UTC
\******************************************************************************/

#ifndef __PARA_DEFS_H__
#define __PARA_DEFS_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "stm32f4xx.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

#define FS 1            // Sampling Rate HZ
#define VcBuf_Len 1024  // length of sampling points
#define atap_Len 400    // length of vad check
#define noise_Len 4000

// VAD
#define frameNum 8                                //
#define frameMovNum 16                            // 帧移
#define frame_len (perSamplingLen / frameNum)     // 帧长
#define frame_mov (perSamplingLen / frameMovNum)  // 帧移，相邻帧交叠部分

#define threshold 0.3
#define perSamplingLen 256
#define perFifoLen 6 * perSamplingLen

#define PI 3.1415926535
#define PWM_PSC 83
#define PWM_OVL 500
#define Radius 0.03
#define WheelDis 0.14

typedef struct {
  u16 mid_val;  //语音段中值 相当于有符号的0值 用于短时过零率计算
  u16 n_thl;  //噪声阈值，用于短时过零率计算
  u16 z_thl;  //短时过零率阈值，超过此阈值，视为进入过渡段。
  u16 s_thl;  //短时累加和阈值，超过此阈值，视为进入过渡段。
} atap_tag;  //自适应参数

typedef struct {
  float x;
  float y;
} position;

typedef struct {
  float delay21;
  float delay31;
  float delay41;
} delay_time;

extern atap_tag   noise_paramter;
extern position   pos_dest;
extern delay_time delay;

typedef enum
{
  AHEAD = 0,
  BACK,
  LEFT,
  RIGHT,
  BREAK,
  GOPOXY,
  VOICE_CTR,
  VOICE_LOCATE,
  AVOID,
  UNKNOW,
} Instructions_TypeDef;

typedef struct {
  Instructions_TypeDef instruction;
  uint8_t              hasResponse;
} Car_Instructions_TypeDef;
// extern parameters
extern uint8_t  SamplingOver_Flag;
extern uint8_t  Voice_Flag;
extern uint16_t adc_result[8 * perFifoLen];
extern float    sig_vf1[2 * VcBuf_Len];
extern float    sig_vf2[2 * VcBuf_Len];
extern float    sig2[2 * VcBuf_Len];
extern float    sig3[2 * VcBuf_Len];
extern float    sig4[2 * VcBuf_Len];

extern uint8_t WriteOverNum;
extern uint8_t NowReadFifoNum;
extern uint8_t NowWriteFifoNum;
extern uint8_t FirstFrameFlag;
extern uint8_t FifoCount;

extern float RWheelAngularVel;
extern float LWheelAngularVel;
extern float LWheelAngularVel_HalfDuty;
extern float RWheelAngularVel_HalfDuty;
extern float CarAngularVel_HalfDuty;
extern float CarLineVel_HalfDuty;

extern uint8_t TermInstr_Come;
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void para_defs_Init(void);

#endif  // __PARA_DEFS_H__
////////////////////////////////// EOF /////////////////////////////////////////
