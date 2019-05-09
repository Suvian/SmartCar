/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * common : [description]
 *
 * filename : common.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 08:33:42 UTC
 * modified : 2019-04-23 09:00:08 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "common.h"
#include <stdarg.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
static char* itoa(int value, char* string, int radix);
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *           -Data   要发送到串口的内容的指针
 *             -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *                   USART2_printf( USART2, "\r\n %d \r\n", i );
 *                   USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, char* Data, ...)
{
  const char* s;
  int         d;
  char        buf[16];

  va_list ap;
  va_start(ap, Data);

  while (*Data != 0)  // 判断是否到达字符串结束符
  {
    if (*Data == 0x5c)  //'\'
    {
      switch (*++Data) {
        case 'r':  //回车符
          USART_SendData(USARTx, 0x0d);
          Data++;
          break;

        case 'n':  //换行符
          USART_SendData(USARTx, 0x0a);
          Data++;
          break;

        default:
          Data++;
          break;
      }
    }

    else if (*Data == '%') {  //
      switch (*++Data) {
        case 's':  //字符串
          s = va_arg(ap, const char*);

          for (; *s; s++) {
            USART_SendData(USARTx, *s);
            while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
              ;
          }

          Data++;

          break;

        case 'd':
          //十进制
          d = va_arg(ap, int);

          itoa(d, buf, 10);

          for (s = buf; *s; s++) {
            USART_SendData(USARTx, *s);
            while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
              ;
          }

          Data++;

          break;

        default:
          Data++;

          break;
      }
    }

    else
      USART_SendData(USARTx, *Data++);

    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
      ;
  }
}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
static char* itoa(int value, char* string, int radix)
{
  int   i, d;
  int   flag = 0;
  char* ptr  = string;

  /* This implementation only works for decimal numbers. */
  if (radix != 10) {
    *ptr = 0;
    return string;
  }

  if (!value) {
    *ptr++ = 0x30;
    *ptr   = 0;
    return string;
  }

  /* if this is a negative value insert the minus sign. */
  if (value < 0) {
    *ptr++ = '-';

    /* Make the value positive. */
    value *= -1;
  }

  for (i = 10000; i > 0; i /= 10) {
    d = value / i;

    if (d || flag) {
      *ptr++ = (char) (d + 0x30);
      value -= (d * i);
      flag = 1;
    }
  }

  /* Null terminate the string. */
  *ptr = 0;

  return string;

} /* NCL_Itoa */

void getInstruction(Car_Instructions_TypeDef* terminal_Instr)
{
  char* pCh;
  char  cCh;
  if (strEsp8266_Fram_Record.InfBit.FramFinishFlag) {
    USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, DISABLE);  //禁用串口接收中断
    strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] =
      '\0';
    (*terminal_Instr).hasResponse = 0;
    if ((pCh = strstr(strEsp8266_Fram_Record.Data_RX_BUF, "terminal_")) != 0) {
      cCh = *(pCh + 9);
      switch (cCh) {
        case 'f':
          (*terminal_Instr).instruction = AHEAD;
          break;
        case 'b':
          (*terminal_Instr).instruction = BACK;
          break;
        case 'l':
          (*terminal_Instr).instruction = LEFT;
          break;
        case 'r':
          (*terminal_Instr).instruction = RIGHT;
          break;
        case 's':
          (*terminal_Instr).instruction = BREAK;
          break;
        case 'V':
          (*terminal_Instr).instruction = VOICE_CTR;
          break;
        case 'L':
          (*terminal_Instr).instruction = VOICE_LOCATE;
          break;
        case 'A':
          (*terminal_Instr).instruction = AVOID;
          break;
        case 'P':
          (*terminal_Instr).instruction = GOPOXY;
          // pos_dest.x=;
          // pos_dest.y=;
          break;
        default:
          (*terminal_Instr).instruction = UNKNOW;
          break;
      }
			}
      strEsp8266_Fram_Record.InfBit.FramLength     = 0;
      strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;

      USART_ITConfig ( ESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
    
  }
  TermInstr_Come=0;
}
////////////////////////////////// EOF /////////////////////////////////////////
