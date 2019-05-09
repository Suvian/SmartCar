/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * common : [description]
 *
 * filename : common.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 08:33:42 UTC
 * modified : 2019-04-22 07:35:58 UTC
\******************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "esp8266.h"
#include "para_defs.h"
#include "stm32f4xx.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
#define WIFI_Usart(fmt, ...) USART_printf(ESP8266_USARTx, fmt, ##__VA_ARGS__)
#define isNewInstrCome() \
  if (TermInstr_Come) {  \
    Break();             \
    TermInstr_Come = 0;  \
    return;              \
  }

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void USART_printf(USART_TypeDef* USARTx, char* Data, ...);

void getInstruction(Car_Instructions_TypeDef* terminal_Instr);

#endif  // __COMMON_H__
////////////////////////////////// EOF /////////////////////////////////////////
