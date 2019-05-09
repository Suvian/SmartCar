
/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * adc : [description]
 *
 * filename : adc.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-21 09:04:33 UTC
 * modified : 2019-04-09 10:28:48 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include "adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
#define ADC_CDR_ADDRESS ((uint32_t) 0x40012308)
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
uint8_t  NVIC_Flag = 0;

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
void ADC_Common_Init()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(
    RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);

  // initialize the GPI0 that be used as ADC input
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*****************DMA config ********************/
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_Channel            = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;  //外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t) adc_result;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //传输方向外设到内存
  DMA_InitStructure.DMA_BufferSize = noise_Len;            //传输长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设递增关闭
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存递增开启

  DMA_InitStructure.DMA_PeripheralDataSize =
    DMA_PeripheralDataSize_HalfWord;  //数据宽度
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

  DMA_InitStructure.DMA_Mode     = DMA_Mode_Circular;     //循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;     //优先级高
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  // DMA FIFO 关闭
  DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  // DMA_Cmd(DMA2_Stream0, ENABLE);  // 使能DMA

  /************* interreput config **************/
  NVIC_Flag = 0;
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  // DMA2_Stream0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;  //子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            // IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化NVIC寄存器
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

  /***ADC Config***/
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;

  /******* initialize the ADC work Mode*********/
  // ADC_CommonInitStructure.ADC_Mode             = ADC_TripleMode_RegSimult;
  // ADC_CommonInitStructure.ADC_TwoSamplingDelay =
  // ADC_TwoSamplingDelay_5Cycles; ADC_CommonInitStructure.ADC_DMAAccessMode =
  // ADC_DMAAccessMode_1; ADC_CommonInitStructure.ADC_Prescaler        =
  // ADC_Prescaler_Div4; ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_CommonInitStructure.ADC_Mode      = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  //选择二分频
  ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 regular channel  configuration ************************************/
  ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;  // 12bit
  ADC_InitStructure.ADC_ScanConvMode         = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion      = 1;

  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  ADC_Init(ADC2, &ADC_InitStructure);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_480Cycles);

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  // ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);
}

// void ADC_Vad_Init()
// {
//   // ADC_Cmd(ADC1, DISABLE);
//   // ADC_Cmd(ADC2, DISABLE);
//   // ADC_Cmd(ADC3, DISABLE);

//   ADC_Cmd(ADC1, DISABLE);

//   DMA_MemoryTargetConfig(DMA2_Stream0, (uint32_t) adc_result, DMA_Memory_0);
//   DMA_SetCurrDataCounter(DMA2_Stream0, atap_Len);
//   // DMA_Cmd(DMA2_Stream0, ENABLE);
//   // DMA中断设置

//   NVIC_Flag = 0;
//   ADC_CommonInitTypeDef ADC_CommonInitStructure;
//   ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
//   ADC_CommonInitStructure.ADC_TwoSamplingDelay =
//   ADC_TwoSamplingDelay_5Cycles; ADC_CommonInitStructure.ADC_DMAAccessMode =
//   ADC_DMAAccessMode_Disabled; ADC_CommonInitStructure.ADC_Prescaler        =
//   ADC_Prescaler_Div8; ADC_CommonInit(&ADC_CommonInitStructure);
//   ADC_MultiModeDMARequestAfterLastTransferCmd(DISABLE);
//   ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

//   ADC_Cmd(ADC1, ENABLE);
// }

void ADC_Voice_Init()
{
  ADC_Cmd(ADC1, DISABLE);

  DMA_MemoryTargetConfig(DMA2_Stream0,
    (uint32_t)(adc_result + FifoCount * perFifoLen), DMA_Memory_0);
  DMA_PeriphAdrConfig(DMA2_Stream0, (uint32_t) & (ADC->CDR));
  DMA_SetCurrDataCounter(DMA2_Stream0, 6 * perSamplingLen);
  // DMA_Cmd(DMA2_Stream0, ENABLE);

  NVIC_Flag = 1;
	FifoCount=0;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_CommonInitStructure.ADC_Mode             = ADC_TripleMode_RegSimult;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div8;
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_DMARequestAfterLastTransferCmd(ADC1, DISABLE);
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
}
void ADC_stop()
{
  DMA_Cmd(DMA2_Stream0, DISABLE);
  ADC_Cmd(ADC1, DISABLE);
 // ADC_Cmd(ADC1, ENABLE);
}
void ADC_start()
{
  ADC_SoftwareStartConv(ADC1);
  DMA_Cmd(DMA2_Stream0, ENABLE);
}

void DMA2_Stream0_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)) {
    if (NVIC_Flag != 0) {
      WriteOverNum++;
        FifoCount=(FifoCount!=7)?(FifoCount+1):0;
        DMA_Cmd(DMA2_Stream0, DISABLE);
        DMA_MemoryTargetConfig(DMA2_Stream0,
          (uint32_t)(adc_result + FifoCount * perFifoLen), DMA_Memory_0);
        DMA_Cmd(DMA2_Stream0, ENABLE);

      if (WriteOverNum == 8) {
      ADC_stop();
      }
    } else {
      ADC_Cmd(ADC1, DISABLE);
      ADC_Cmd(ADC1, ENABLE);
      DMA_Cmd(DMA2_Stream0, DISABLE);
    }

    SamplingOver_Flag = 1;
  }

  DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
}

uint32_t ADC_GetDMA2ITStatus(uint32_t DMA_FLAG)
{
  return DMA_GetITStatus(DMA2_Stream0, DMA_FLAG);
}

// uint32_t GetCR(void){
//   return DMA_GetCRStatus(DMA2_Stream0);
// }

///////ADC_Init(ADC1, &ADC_InitStructure);/////////////////////////// EOF /////////////////////////////////////////
