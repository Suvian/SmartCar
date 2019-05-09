/********     VAD.C       *******/
/*******************************************************************************
 * Copyright (c) 2019, Induspotis, Inc.
 * All rights reserved.
 *
 * vad : [description]
 *
 * filename : vad.c
 * author   : Sluvian(Sluvian@hit.edu.cn)
 * create   : 2019-03-28 06:28:25 UTC
 * modified : 2019-04-10 01:51:31 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
// 
#include "vad.h"
////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
// 
#define n_thl_ratio 1  //��������ϵ�� n_thl=n_max_mean*n_thl_ratio
#define s_thl_ratio 11 / 10  //��ʱ�����о�����ϵ�� s_thl=sum_mean*s_thl_ratio
#define z_thl_ratio 2 / 160  //��ʱ������ �о�����ϵ�� ����

// #define atap_frm_t 30  //������������Ӧʱ��֡���� ms
#define atap_frm_len 400  //������������Ӧ֡����

//#define v_durmin_t 80  //��Ч�������ʱ������ ms
#define v_durmin_f 10  //��Ч�������֡��
#define Vref 3.3

// extern float sig_vf1[];
// extern float sig_vf2[];
// extern float sig2[];
// extern float sig3[];
// extern float sig4[];
// extern uint16_t adc_result[];
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
// 

////////////////////////////////////////////////////////////////////////////////
// Classes
// 

////////////////////////////////////////////////////////////////////////////////
// Functions
// 

void noise_atap(const u16* noise, u16 n_len, atap_tag* atap)
{
  u32 h, i;
  u32 n_max;
  u32 max_sum;  //ÿһ֡�������ֵ �ۼ�ȡƽ�� ��������ֵ
  u32 n_sum;    //������ֵ֮�� ��ƽ��ֵ ȷ����(��)ֵ
  u32 mid;      //��ֵ
  u32 abs;      //����ֵ
  u32 abs_sum;  //����ֵ��
  u32 frm_num;

  if ((n_len % atap_frm_len) != 0)  //�������
  {
    return;
  }
  frm_num = n_len / atap_frm_len;

  n_sum   = 0;
  max_sum = 0;
  for (i = 0; i < n_len; i++) {
    n_sum += *(noise + i);
  }
  mid = n_sum / i;

  abs_sum = 0;
  for (i = 0; i < n_len; i += atap_frm_len) {
    n_max = 0;
    for (h = 0; h < atap_frm_len; h++) {
      abs = (*(noise + i + h) > mid) ? (*(noise + i + h) - mid)
                                     : (mid - *(noise + i + h));
      if (abs > n_max)  //ȡÿ֡������ֵ
      {
        n_max = abs;
      }
      abs_sum += abs;
    }
    max_sum += n_max;
    // USART1_printf("n_max=%d ", n_max);
    // USART1_printf("max_sum=%d\r\n", max_sum);
  }

  abs_sum /= (n_len / frame_len);
  max_sum /= frm_num;
  atap->mid_val = mid;
  // atap->n_thl   = max_sum * n_thl_ratio;
  atap->n_thl   = (uint16_t)(threshold/3.3*4096);
  atap->s_thl   = abs_sum * s_thl_ratio;
  atap->z_thl   = frame_len * z_thl_ratio / n_thl_ratio;
}

uint8_t VAD(const u16* vc, u16 buf_len, atap_tag* atap_arg)
{
  // buf_len ��������������  �� atap_arg ��������

  u16 sum_count = 0;  //ǰ�˹��ɶγ�������ֵ����֡��

  u32 h,i;
//  u32 frm_sum;  // ��ʱ����ֵ��
//  u32 frm_zero; // ��ʱ����(����)��
  u32 a_thl;  // ������ֵ
  u32 b_thl;  // ������ֵ
  
//  u32 frm_con=0;  //֡����
  
  a_thl=atap_arg->mid_val+atap_arg->n_thl;
  b_thl=atap_arg->mid_val-atap_arg->n_thl;
  
  
  for(i=0;i<(buf_len-frame_len);i+=(frame_len-frame_mov))
  {

uint8_t overThlCount=0;
//    frm_sum=0;
    for(h=0;h<frame_len;h++)//��ʱ����ֵ��
    {
      overThlCount=((*(vc+6*(i+h))>a_thl)||(*(vc+6*(i+h))<b_thl))?(overThlCount+1): overThlCount;
    }
    
    // frm_sum=0;
    // for(h=0;h<frame_len;h++)//��ʱ����ֵ��
    // {
    //   frm_sum+=(*(vc+6*(i+h))>(atap_arg->mid_val))?(*(vc+6*(i+h))-(atap_arg->mid_val)):((atap_arg->mid_val)-*(vc+6*(i+h)));
    // }
    
    // frm_zero=0;
    // for(h=0;h<(frame_len-1);h++)//��ʱ��������
    // {
    //   if(*(vc+i+h)>=a_thl)      //����������ֵ
    //   {
    //     last_sig=2;
    //   }
    //   else if(*(vc+i+h)<b_thl)  //С��������ֵ
    //   {
    //     last_sig=1;
    //   }
      
    //   if(*(vc+i+h+1)>=a_thl)
    //   {
    //     if(last_sig==1)
    //     {
    //       frm_zero++;
    //     }
    //   }
    //   else if(*(vc+i+h+1)<b_thl)
    //   {
    //     if(last_sig==2)
    //     {
    //       frm_zero++;
    //     }
    //   }
    // }
    
    // if((frm_sum>(atap_arg->s_thl))||(frm_zero>(atap_arg->z_thl)))

    if(overThlCount>frame_len/2)
    //������һ����������������ֵ
    {
      sum_count++;
      if((!(sum_count<3))&(h<4)){
        FirstFrameFlag=1;
      }
    }
  }
  return (sum_count>v_durmin_f)?1:0;

}
// clang-format on
 void Sampling_to_vol(uint16_t address,uint16_t Len,atap_tag* atap_arg){
  int16_t mid_zero = atap_arg->mid_val;
  for(int i=0;i<Len;i++){
    sig_vf1[2*i]=((float)((int)(*(adc_result+address+6*i  ))-(int)mid_zero))/(float)4095*(Vref);
    sig_vf2[2*i]=((float)((int)(*(adc_result+address+6*i+3))-(int)mid_zero))/(float)4095*(Vref);
    sig2[2*i]=   ((float)((int)(*(adc_result+address+6*i+1))-(int)mid_zero))/(float)4095*(Vref);
    sig3[2*i]=   ((float)((int)(*(adc_result+address+6*i+2))-(int)mid_zero))/(float)4095*(Vref);
    sig4[2*i]=   ((float)((int)(*(adc_result+address+6*i+4))-(int)mid_zero))/(float)4095*(Vref);
    sig_vf1[2*i+1]=0;
    sig_vf2[2*i+1]=0;
    sig2[2*i+1]=0;
    sig3[2*i+1]=0;
    sig4[2*i+1]=0;
  }
 }
////////////////////////////////// EOF /////////////////////////////////////////

