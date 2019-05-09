/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * motor : [description]
 *
 * filename : motor.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-29 06:50:28 UTC
 * modified : 2019-04-23 12:03:29 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "motor.h"
#include "arm_math.h"
#include "common.h"
#include "delay.h"
#include "math.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
enum WheelStatus
{
  STOP = 0,
  FORWARD,
  REVESE
};

enum WheelStatus RState;
enum WheelStatus LState;

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

void WSpeed(float* RAngVel, float* LAngVel)
{
  switch (RState) {
    case STOP:
      *RAngVel = 0;
      break;
    case FORWARD:
      *RAngVel = RWheelAngularVel;
      break;
    case REVESE:
      *RAngVel = (-RWheelAngularVel);
      break;
    default:
      break;
  }
  switch (LState) {
    case STOP:
      *LAngVel = 0;
      break;
    case FORWARD:
      *LAngVel = LWheelAngularVel;
      break;
    case REVESE:
      *LAngVel = (-LWheelAngularVel);
      break;
    default:
      break;
  }
}

void GoLeft()
{
  RState = FORWARD;
  LState = REVESE;
  RIGHT1 = 1;
  RIGHT2 = 0;
  LEFT1  = 0;
  LEFT2  = 1;
}
void GoRight()
{
  LState = FORWARD;
  RState = REVESE;

  RIGHT1 = 0;
  RIGHT2 = 1;
  LEFT1  = 1;
  LEFT2  = 0;
}
void GoAhead()
{
  LState = FORWARD;
  RState = FORWARD;

  RIGHT1 = 1;
  RIGHT2 = 0;
  LEFT1  = 1;
  LEFT2  = 0;
}
void GoBack()
{
  LState = REVESE;
  RState = REVESE;

  // dir of wheel
  RIGHT1 = 0;
  RIGHT2 = 1;
  LEFT1  = 0;
  LEFT2  = 1;
}
void Break()
{
  LState = FORWARD;
  RState = FORWARD;

  RIGHT1 = 1;
  RIGHT2 = 1;
  LEFT1  = 1;
  LEFT2  = 1;
}
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

// void WheelSet()
// {
//   switch (RState) {
//     case STOP:
//       RIGHT1 = 1;
//       RIGHT2 = 1;
//       break;
//     case FORWARD:
//       RIGHT1 = 0;
//       RIGHT2 = 1;
//       break;
//     case REVESE:
//       RIGHT1 = 1;
//       RIGHT2 = 0;
//       break;
//     default:
//       RIGHT1 = 1;
//       RIGHT2 = 1;
//       break;
//   }

//   switch (LState) {
//     case STOP:
//       LEFT1 = 1;
//       LEFT2 = 1;
//       break;
//     case FORWARD:
//       LEFT1 = 0;
//       LEFT2 = 1;
//       break;
//     case REVESE:
//       LEFT1 = 1;
//       LEFT2 = 0;
//       break;
//     default:
//       LEFT1 = 1;
//       LEFT2 = 1;
//       break;
//   }
// }

float GetAngularVel(void)
{
  float RAngVel = 0;
  float LAngVel = 0;
  WSpeed(&RAngVel, &LAngVel);
  float speed = Radius * (LAngVel - RAngVel) / WheelDis;
  return speed;
}

float GetLineVel(void)
{
  float RAngVel = 0;
  float LAngVel = 0;
  WSpeed(&RAngVel, &LAngVel);
  float speed = Radius * (RAngVel + LAngVel) / 2;
  return speed;
}

void Turn(float degree)
{
  float angular = degree / 180 * PI;
  // Right_SetDuty(PWM_OVL / 2);
  // Left_SetDuty(PWM_OVL / 2);
  if (angular < 0) {
    angular = (-angular);
    GoLeft();
  } else {
    GoRight();
  }

  // float speed = GetAngularVel();
  int time = angular / CarAngularVel_HalfDuty * 1000 * Correction;
  delay_ms(time);
  Break();
}
void GoStraight(float distance)
{
  int time = (distance / CarLineVel_HalfDuty) * 1000 * Correction;
  GoAhead();
  // delay_ms(200);
  // float speed = GetLineVel();
  // int   time  = (distance / speed) * 1000;
  delay_ms(time);
  Break();
}
void GoPoxy(position Poxy)
{
  float angular     = (PI / 2 - atan2(Poxy.x, Poxy.y));
  float distance    = sqrt(Poxy.x * Poxy.x + Poxy.y * Poxy.y);
  int   turnTime    = (angular / CarAngularVel_HalfDuty) * 1000 * Correction;
  int   goAheadTime = (distance / CarLineVel_HalfDuty) * 1000 * Correction;
  // Right_SetDuty(PWM_OVL / 2);
  // Left_SetDuty(PWM_OVL / 2);
  if (angular < 0) {
    angular = (-angular);
    GoLeft();
  } else {
    GoRight();
  }
  // float speed = GetAngularVel();
  // int   time  = (angular / CarAngularVel_HalfDuty) * 1000;
  delay_ms(turnTime);
  Break();
  // time = (distance / CarLineVel_HalfDuty) * 1000;
  GoAhead();
  delay_ms(goAheadTime);
  Break();
}

void SetSpeed(u16 wheel, u16 speed) {}

void Avoid()
{
  while (1) {
    if (!(RINFR2 && LINFR2)) {
      if (RINFR1) {
        GoRight();
        while (RINFR2 && LINFR2) {
          isNewInstrCome();
        }
      } else if (LINFR1) {
        GoLeft();
        while (RINFR2 && LINFR2) {
          isNewInstrCome();
        }
      } else {
        GoBack();
        while (RINFR1 || LINFR1) {
          isNewInstrCome();
        }
        if (RINFR1) {
          GoRight();
          while (RINFR2 && LINFR2) {
            isNewInstrCome();
          }
        } else {
          GoLeft();
          while (RINFR2 && LINFR2) {
            isNewInstrCome();
          }
        }
      }
    } else {
      GoAhead();
      isNewInstrCome();
    }
  }
}
////////////////////////////////// EOF /////////////////////////////////////////
