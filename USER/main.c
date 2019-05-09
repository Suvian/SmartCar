/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * main : [description]
 *
 * filename : main.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-28 02:17:53 UTC
 * modified : 2019-04-24 08:03:38 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//

// #include "adc.h"
#include "common.h"
#include "delay.h"
#include "motor.h"
#include "motordriver.h"
#include "para_defs.h"
#include "speechcontrol.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "voice_locate.h"
#include "wifi.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constant

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
//

int main(void)
{
  // initial all parts
  // para_defs_Init();

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart_init(115200);
  delay_init(168);
  printf("init\r\n");
  LED_Init();
  WIFI_Init();
  VoiceLoc_Init();
  Speech_Init();
  Motor_Init();

  // delay_ms(1000);
  // GoAhead();
  // while(1);

  // 语音模块初始化
  // Wifi_Test();

  Car_Instructions_TypeDef terminal_Instr;
  terminal_Instr.instruction                   = BREAK;
  terminal_Instr.hasResponse                   = 1;
  strEsp8266_Fram_Record.InfBit.FramLength     = 0;
  strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
  printf("ALL module is ready begin work!!!!!!\r\n");
  // Avoid();
  while (1) {
    getInstruction(&terminal_Instr);
    if (!terminal_Instr.hasResponse) {
      terminal_Instr.hasResponse = 1;
      switch (terminal_Instr.instruction) {
        case AHEAD:
          // WIFI_Usart("AT+CIPSEND=0,25\r\n");
          // printf("instruction is :AHEAD\r\n");
          GoAhead();
          break;
        case BACK:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :BACK\r\n");
          GoBack();
          break;
        case LEFT:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is LEFT:\r\n");
          GoLeft();
          break;
        case RIGHT:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :RIGHT\r\n");
          GoRight();
          break;
        case BREAK:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :BREAK\r\n");
          Break();
          break;
        case GOPOXY:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :GOPOXY\r\n");
          GoPoxy(pos_dest);
          break;
        case VOICE_CTR:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :VOICE_CTR\r\n");
          SpeechCtr();
          break;
        case VOICE_LOCATE:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :VOICE_LOCATE\r\n");
          VoiceLoc();
          break;
        case AVOID:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is :AVOID\r\n");
          Avoid();
          break;
        case UNKNOW:
          // WIFI_Usart("AT+CIPSEND=0,40\r\n");
          // printf("instruction is : unknow\r\n");
          Break();
          break;
        default:
          Break();
          break;
      }
    }
  }
}
////////////////////////////////// EOF /////////////////////////////////////////
