/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * calculate_poxy : [description]
 *
 * filename : calculate_poxy.c
 * author   : wy(wy@hit.edu.cn)
 * create   : 2019-03-18 02:50:01 UTC
 * modified : 2019-04-10 01:51:00 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "calculate_poxy.h"
#include "arm_math.h"
#include "math.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
#define v 340             // the speed of voice
#define correct_factor 1  //
#define d 0.15            // the position of mic

#define abs(value) (value > 0) ? value : (-value)
#define eps 0.00000001

// extern float sig_vf1[];
// extern float sig_vf2[];
// extern float sig2[];
// extern float sig3[];
// extern float sig4[];
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
float delay_two_sigs(float* sig, float* sig_vf, uint16_t SamplingNum)
{
  int ifftFlag = 0;
  // float fft_sig[SamplingNum];
  // float fft_sig_vf[SamplingNum];

  /******FFT********/
  arm_cfft_radix4_instance_f32 scfft;
  arm_cfft_radix4_instance_f32 scfft_vf;
  // arm_cfft_instance_f32        scfft;
  // arm_cfft_instance_f32        scfft_vf;

  arm_cfft_radix4_init_f32(&scfft, SamplingNum, ifftFlag, 1);
  arm_cfft_radix4_init_f32(&scfft_vf, SamplingNum, ifftFlag, 1);

  arm_cfft_radix4_f32(&scfft, sig);
  arm_cfft_radix4_f32(&scfft_vf, sig_vf);
  // arm_cfft_f32(&scfft,sig,0,1);
  // arm_cfft_f32(&scfft_vf,sig_vf,0,1);

  /**********FFT*CONJ(FFT_VFF)********/
  float tmp1[2 * SamplingNum];
  int   i;
  for (i = 0; i < SamplingNum; i += 2) {
    tmp1[i]     = (sig[i] * sig_vf[i] + sig[i + 1] * sig_vf[i + 1]);
    tmp1[i + 1] = (sig[i + 1] * sig_vf[i] - sig[i] * sig_vf[i + 1]);

    // tmp1[SamplingNum - 1 + i] =
    //   sig[SamplingNum - 2 + i] * sig_vf[SamplingNum - 1 + i]
    //   - sig[SamplingNum - 1 + i] * sig_vf[SamplingNum - 2 + i];
    // tmp1[SamplingNum - 2 + i] =
    //   sig[SamplingNum - 2 + i] * sig_vf[SamplingNum - 2 + i]
    //   + sig[SamplingNum - 1 + i] * sig_vf[SamplingNum - 1 + i];
  }

  float tmp_mag[SamplingNum];
  arm_cmplx_mag_f32(tmp1, tmp_mag, SamplingNum);
  // GCC PHAT
  for (i = 0; i < SamplingNum; i++) {
    tmp1[i]     = tmp1[i] / (tmp_mag[i] + eps);
    tmp1[i + 1] = tmp1[i + 1] / (tmp_mag[i] + eps);
  }

  // arm_cfft_instance_f32 scifft;
  // scifft.fftLen = SamplingNum;
  // arm_cfft_f32(&scifft, tmp1, 1, 0);

  /***iFFT ***/
  ifftFlag = 1;
  arm_cfft_radix4_instance_f32 scfft_ifft;
  arm_cfft_radix4_init_f32(&scfft_ifft, SamplingNum, 1, 1);
  arm_cfft_radix4_f32(&scfft_ifft, tmp1);
  // arm_cfft_f32(&scfft, tmp1, ifftFlag, 1);

  /******looking for the max position******/
  float check = 0;
  int   count = 0;
  for (i = 0; i < 2 * SamplingNum; i+=2) {
    float tmp = abs(tmp1[i]);
    count     = (check < tmp) ? i : count;
    check     = (check < tmp) ? tmp : check;
  }

  if (count < SamplingNum) {
    count = count + SamplingNum;
  } else {
    count = count - SamplingNum;
  }

  float delay = (count - SamplingNum)/2/FS;
  return delay;
}

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
//

void delay_get(delay_time* delay)
{
  delay->delay21 = delay_two_sigs(sig2, sig_vf1, VcBuf_Len);
  delay->delay31 = delay_two_sigs(sig3, sig_vf1, VcBuf_Len);
  delay->delay41 = delay_two_sigs(sig4, sig_vf2, VcBuf_Len);
}

void pos_get(delay_time* delay, position* poxy)
{
  float delay21 = delay->delay21;
  float delay31 = delay->delay31;
  float delay41 = delay->delay41;
  float Time1 =
    ((delay21) * (delay21) + (delay41) * (delay41) - (delay31) * (delay31))
    / (2 * (delay31 - delay21 - delay41));

  poxy->x = (v * v * delay31 * (delay31 + 2 * Time1)) / (4 * d);
  poxy->y =
    (v * v * (delay41 - delay21) * (delay41 + delay21 + 2 * Time1)) / (4 * d);
}

////////////////////////////////// EOF /////////////////////////////////////////
