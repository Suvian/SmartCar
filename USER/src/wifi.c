/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * wifi : [description]
 *
 * filename : wifi.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 07:15:28 UTC
 * modified : 2019-04-22 08:40:43 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "wifi.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//

////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//

void ESP8266_AT_Test(void)
{
  char count = 0;

  ESP8266_RST_HIGH_LEVEL();
  delay_ms(1000);
  while (count < 10) {
    if (ESP8266_Cmd("AT", "OK", NULL, 200))
      return;
    ESP8266_Rst();
    ++count;
  }
}

uint8_t ESP8266_Inquire_ApIp(char* pApIp, uint8_t ucArrayLength)
{
  char  uc;
  char* pCh;
  ESP8266_Cmd("AT+CIFSR", "OK", 0, 200);
  pCh = strstr(strEsp8266_Fram_Record.Data_RX_BUF, "APIP,\"");
  if (pCh)
    pCh += 6;
  else
    return 0;
  for (uc = 0; uc < ucArrayLength; uc++) {
    pApIp[uc] = *(pCh + uc);

    if (pApIp[uc] == '\"') {
      pApIp[uc] = '\0';
      break;
    }
  }
  return 1;
}
////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//

/*
 *the function for configing wifi work mode
 */

bool ESP8266_Cmd(char* cmd, char* reply1, char* reply2, u32 waittime)
{
  strEsp8266_Fram_Record.InfBit.FramLength = 0;  //从新开始接收新的数据包
  ESP8266_Usart("%s\r\n", cmd);

  if ((reply1 == 0) && (reply2 == 0))  //不需要接收数据
  {
    return true;
  }
  // recive response
  delay_ms(waittime);  //延时
  strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] =
    '\0';
  PC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);

  if ((reply1 != 0) && (reply2 != 0)) {
    return ((bool) strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply1)
            || (bool) strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply2));
  } else if (reply1 != 0) {
    return ((bool) strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply1));
  } else {
    return ((bool) strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply2));
  }
}

void ESP8266_Rst()
{
#if 0
     ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );
#else
  ESP8266_RST_LOW_LEVEL();
  delay_ms(500);
  ESP8266_RST_HIGH_LEVEL();
#endif
}

bool ESP8266_NetMode_Set(ENUM_Net_ModeTypeDef Mode)
{
  switch (Mode) {
    case STA:
      return ESP8266_Cmd("AT+CWMODE=1", "OK", "no change", 200);
    case AP:
      return ESP8266_Cmd("AT+CWMODE=2", "OK", "no change", 200);
    case STA_AP:
      return ESP8266_Cmd("AT+CWMODE=3", "OK", "no change", 200);
    default:
      return false;
  }
}

bool ESP8266_BuildAP(
  char* pSSID, char* pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode)
{
  char cCmd[120];
  sprintf(cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode);
  return ESP8266_Cmd(cCmd, "OK", 0, 200);
}

bool ESP8266_TCPSever_Set(
  FunctionalState enumMode, char* pPortNum, char* pTimeOver)
{
  char cCmd1[120], cCmd2[120];

  if (enumMode) {
    sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum);
    sprintf(cCmd2, "AT+CIPSTO=%s", pTimeOver);
    return (ESP8266_Cmd(cCmd1, "OK", 0, 200) && ESP8266_Cmd(cCmd2, "OK", 0, 200));
  } else {
    sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum);
    return ESP8266_Cmd(cCmd1, "OK", 0, 200);
  }
}

bool ESP8266_MultiUser_Set(FunctionalState enumEnUnvarnishTx)
{
  char cStr[20];
  sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
  return ESP8266_Cmd(cStr, "OK", 0, 200);
}

bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char* pStr,
  u32 ulStrLength, ENUM_ID_NO_TypeDef ucId)
{
  char cStr[20];
  bool bRet = false;

  if (enumEnUnvarnishTx) {
    ESP8266_Usart("%s", pStr);
    bRet = true;
  } else {
    if (ucId < 5) {
      sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);
    } else {
      sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);
    }
    ESP8266_Cmd(cStr, "> ", 0, 100);
    bRet = ESP8266_Cmd(pStr, "SEND OK", 0, 500);
  }
  return bRet;
}

bool ESP8266_APIP_Set(char* pApIp)
{
  char cCmd[30];

  sprintf(cCmd, "AT+CIPAP=\"%s\"", pApIp);

  if (ESP8266_Cmd(cCmd, "OK", 0, 200)) {
    return 1;
  } else {
    return 0;
  }
}
 bool ESP8266_Echo_Set(FunctionalState ENorDIS){
    char cStr[20];
  sprintf(cStr, "ATE%d", (ENorDIS ? 1 : 0));
  return ESP8266_Cmd(cStr, "OK", 0, 200);
 }

void WIFI_Init()
{
  ESP8266_Init();
  ESP8266_CH_ENABLE();
  ESP8266_AT_Test();
  ESP8266_NetMode_Set(AP);

  while (!ESP8266_APIP_Set(ESP8266_TcpServer_IP))
    ;

  while (!ESP8266_BuildAP(
    ESP8266_BulitApSsid, ESP8266_BulitApPwd, ESP8266_BulitApEcn))
    ;

  ESP8266_MultiUser_Set(ENABLE);
  // ESP8266_Echo_Set(DISABLE);
  while (!ESP8266_TCPSever_Set(
    ENABLE, ESP8266_TcpServer_Port, ESP8266_TcpServer_OverTime))
    ;
  strEsp8266_Fram_Record.InfBit.FramLength     = 0;
  strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
}

/****************************test****************************/

#ifdef ESP8266_TEST
void Wifi_Test()
{
  ESP8266_Init();

  char cStr[100] = {0};

  printf("\r\n正在配置 ESP8266 ......\r\n");

  ESP8266_CH_ENABLE();
  ESP8266_AT_Test();
  ESP8266_NetMode_Set(AP);

  while (!ESP8266_APIP_Set(ESP8266_TcpServer_IP))
    ;

  while (!ESP8266_BuildAP(
    ESP8266_BulitApSsid, ESP8266_BulitApPwd, ESP8266_BulitApEcn))
    ;

  ESP8266_MultiUser_Set(ENABLE);

  while (!ESP8266_TCPSever_Set(
    ENABLE, ESP8266_TcpServer_Port, ESP8266_TcpServer_OverTime))
    ;

  ESP8266_Inquire_ApIp(cStr, 20);
  printf("\r\n the name of WIFI is:%s，passwords are open for you:\r\nAP IP "
         "is:%s，the port is:%s\r\n",
    ESP8266_BulitApSsid, cStr, ESP8266_TcpServer_Port);

  strEsp8266_Fram_Record.InfBit.FramLength     = 0;
  strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
  while (1) {
    if (strEsp8266_Fram_Record.InfBit.FramFinishFlag) {
      USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, DISABLE);  //禁用串口接收中断
      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] =
        '\0';

      printf("\r\n%s\r\n", strEsp8266_Fram_Record.Data_RX_BUF);

      strEsp8266_Fram_Record.InfBit.FramLength     = 0;
      strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;

      USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, ENABLE);  //使能串口接收中断
    }
  }
}
#endif
////////////////////////////////// EOF /////////////////////////////////////////
