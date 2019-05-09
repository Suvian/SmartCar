/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * ld3320 : [description]
 *
 * filename : ld3320.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-23 01:23:59 UTC
 * modified : 2019-04-23 04:32:17 UTC
\******************************************************************************/

#ifndef __LD3320_H__
#define __LD3320_H__

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

// clang-format off
#define LD3320_USART_IRQ                    USART2_IRQn
#define LD3320_USART_INT_FUN                USART2_IRQHandler

#define LD3320_USART_BAUD_RATE              115200

#define LD3320_USARTx                       USART2
#define LD3320_USART_APBxClock_FUN          RCC_APB1PeriphClockCmd
#define LD3320_USART_CLK                    RCC_APB1Periph_USART2
#define LD3320_USART_GPIO_APBxClock_FUN     RCC_AHB1PeriphClockCmd
#define LD3320_USART_GPIO_CLK               RCC_AHB1Periph_GPIOD    
#define LD3320_USART_PORT                   GPIOD   
#define LD3320_USART_TX_PIN                 GPIO_Pin_5
#define LD3320_USART_TX_AFPIN               GPIO_PinSource5
#define LD3320_USART_RX_PIN                 GPIO_Pin_6
#define LD3320_USART_RX_AFPIN               GPIO_PinSource6
// clang-format on

extern uint8_t speech_Instr[];
extern uint8_t    speechInstrCome;
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void LD3320_USART_Init();
void LD3320_Init();

#endif  // __LD3320_H__
////////////////////////////////// EOF /////////////////////////////////////////
