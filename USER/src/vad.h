#ifndef _VAD_H
#define _VAD_H

#include "stm32f4xx.h"
#include "para_defs.h"

// typedef struct
// {
// 	u16 *start;	//��ʼ��
// 	u16 *end;	//������
// }valid_tag;	//��Ч������

void noise_atap(const u16* noise,u16 n_len,atap_tag* atap);
uint8_t VAD(const u16 *vc, u16 buf_len, atap_tag *atap_arg);
void Sampling_to_vol(uint16_t address,uint16_t Len,atap_tag* atap_arg);

#endif
