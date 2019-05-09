/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * voicecontrol : [description]
 *
 * filename : voicecontrol.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-22 03:14:53 UTC
 * modified : 2019-04-23 09:32:09 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "speechcontrol.h"
#include "ld3320.h"
#include "motor.h"
#include "timer.h"
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
void Speech_Init()
{
  LD3320_Init();
  Speech_TIM_Init(run_Time_ms * 10 - 1, 8400 - 1);
}
void SpeechCtr()
{
          printf("speech work begin\r\n");
  while (1) {
    if (speechInstrCome ) {
          printf("new instruction come\r\n");
      switch (speech_Instr[0]) {
        case 1:
          GoAhead();
          // printf("go ahead\r\n");
          break;
        case 2:
          // printf("back\r\n");
          GoBack();
          break;
        case 3:
          // printf("stop\r\n");
          break;
        case 4:
          // printf("turn left\r\n");
          Turn(-90);
          break;
        case 5:
          // printf("turn right\r\n");
          Turn(90);
          break;
        case 6:
          break;
        default:
          break;
      }
      SPEECH_TIMx->CNT = 0;
      runTime_Flag     = 0;
      speechInstrCome  = 0;
    }
    if (runTime_Flag) {
      runTime_Flag = 0;
      Break();
    }
    if (TermInstr_Come) {
      return;
    }
  }
}
////////////////////////////////// EOF /////////////////////////////////////////
