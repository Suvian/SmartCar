/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * voice_locate : [description]
 *
 * filename : voice_locate.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-15 09:35:58 UTC
 * modified : 2019-05-05 02:23:51 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "voice_locate.h"
#include "adc.h"
#include "calculate_poxy.h"
#include "common.h"
#include "motor.h"
#include "para_defs.h"
#include "vad.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void VoiceLoc()
{
  ADC_Voice_Init();
  while (1) {
    pos_dest.x = 0;
    pos_dest.y = 0;
    ADC_start();
    delay_ms(50);

    // judge the voice is come or not
    while (WriteOverNum != 0) {
      Voice_Flag = VAD(adc_result + perFifoLen * NowReadFifoNum, perSamplingLen,
        &noise_paramter);
      isNewInstrCome();
      if (Voice_Flag != 0) {
        while (WriteOverNum < 4) {
          isNewInstrCome();
        }
        // ADC_stop();
        break;
      }

      WriteOverNum--;
      if (!WriteOverNum) {
        delay_ms(50);
      }

      (NowReadFifoNum != 7) ? (++NowReadFifoNum) : (NowReadFifoNum = 0);
    }
    printf("Voice appear \n");

    uint16_t address = perFifoLen * (NowReadFifoNum - FirstFrameFlag);
    Sampling_to_vol(address, VcBuf_Len, &noise_paramter);

    delay_get(&delay);
    // printf("delay12 is:%f \r\n", delay.delay21);
    // printf("delay13 is:%f \r\n", delay.delay31);
    // printf("delay14 is:%f \r\n", delay.delay41);

    pos_get(&delay, &pos_dest);
    // printf("the result is:\r\n");
    // printf("x=%f\r\n", pos_dest.x);
    // printf("y=%f\r\n", pos_dest.y);
    // WIFI_Usart("x=%f,y=%f", pos_dest.x, pos_dest.y);
    GoPoxy(pos_dest);
  }
}
void VoiceLoc_Init()
{
  ADC_Common_Init();
  // WIFI_Usart("voice locate system start work\r\n");
  // WIFI_Usart("please keep quit for sampling the noise_paramter\r\n" );
  delay_ms(500);
  ADC_start();
  while (!SamplingOver_Flag)
    ;  // delay()

  SamplingOver_Flag = 0;
  noise_atap(adc_result, noise_Len, &noise_paramter);
}
////////////////////////////////// EOF /////////////////////////////////////////
