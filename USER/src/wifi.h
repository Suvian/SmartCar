/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * wifi : [description]
 *
 * filename : wifi.h
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-04-18 07:15:28 UTC
 * modified : 2019-04-22 07:07:05 UTC
\******************************************************************************/

#ifndef __WIFI_H__
#define __WIFI_H__

/*****test mode*****/
// #define ESP8266_TEST

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "common.h"
#include "esp8266.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
typedef enum
{
  STA,
  AP,
  STA_AP
} ENUM_Net_ModeTypeDef;

typedef enum
{
  enumTCP,
  enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum
{
  Multiple_ID_0 = 0,
  Multiple_ID_1 = 1,
  Multiple_ID_2 = 2,
  Multiple_ID_3 = 3,
  Multiple_ID_4 = 4,
  Single_ID_0   = 5,
} ENUM_ID_NO_TypeDef;

typedef enum
{
  OPEN         = 0,
  WEP          = 1,
  WPA_PSK      = 2,
  WPA2_PSK     = 3,
  WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

// clang-format off
#define ESP8266_Usart(fmt, ...)   USART_printf(ESP8266_USARTx, fmt, ##__VA_ARGS__)
#define PC_Usart(fmt, ...)        printf(fmt, ##__VA_ARGS__)
// clang-format on

#define ESP8266_BulitApSsid "SmartCar"  //要建立的热点的名称
#define ESP8266_BulitApEcn OPEN         //要建立的热点的加密方式
#define ESP8266_BulitApPwd "samrtcar"   //要建立的热点的密钥

#define ESP8266_TcpServer_IP "192.168.123.169"  //服务器开启的IP地址
#define ESP8266_TcpServer_Port "8080"           //服务器开启的端口

#define ESP8266_TcpServer_OverTime "1800"  //服务器超时时间（单位：秒）

/******TEST define paramater***/

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
// clang-format off
bool ESP8266_Cmd            (char* cmd, char* reply1, char* reply2, u32 waittime);
void ESP8266_Rst            (void);
bool ESP8266_NetMode_Set    (ENUM_Net_ModeTypeDef enumMode);
bool ESP8266_BuildAP        (char* pSSID, char* pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode);
bool ESP8266_TCPSever_Set   (FunctionalState enumMode, char* pPortNum, char* pTimeOver);
bool ESP8266_MultiUser_Set  (FunctionalState enumEnUnvarnishTx);
bool ESP8266_SendString     (FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId);
bool ESP8266_APIP_Set       (char* pApIp);
bool ESP8266_Echo_Set       (FunctionalState ENorDIS);
void WIFI_Init(void);

//test function
#ifdef ESP8266_TEST
void Wifi_Test(void);
#endif
#endif  // __WIFI_H__
////////////////////////////////// EOF /////////////////////////////////////////
