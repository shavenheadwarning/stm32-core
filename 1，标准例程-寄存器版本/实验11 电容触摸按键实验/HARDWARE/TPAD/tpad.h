#ifndef __TPAD_H
#define __TPAD_H
#include "sys.h"		
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//TPAD ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
//���ֵӦ����ÿ�ο�����ʱ�򱻳�ʼ��һ��
extern vu16 tpad_default_val;
							   	    
void TPAD_Reset(void);
u16  TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8  TPAD_Init(u8 psc);
u8  TPAD_Scan(u8 mode);
void TIM3_CH4_Cap_Init(u16 arr,u16 psc);   
#endif























