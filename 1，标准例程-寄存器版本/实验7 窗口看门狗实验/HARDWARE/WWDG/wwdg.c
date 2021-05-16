#include "wwdg.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//���ڿ��Ź� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). 
void WWDG_Init(u8 tr,u8 wr,u8 fprer) 
{    
	RCC->APB1ENR|=1<<11; 	//ʹ��wwdgʱ�� 
	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.     
	WWDG->CFR|=fprer<<7;    //PCLK1/4096�ٳ�2^fprer 
	WWDG->CFR&=0XFF80;      
	WWDG->CFR|=wr;     		//�趨����ֵ      
	WWDG->CR|=WWDG_CNT; 	//�趨������ֵ 
	WWDG->CR|=1<<7;  		//�������Ź�      
	MY_NVIC_Init(2,3,WWDG_IRQn,2);//��ռ2�������ȼ�3����2     
	WWDG->SR=0X00; 			//�����ǰ�����жϱ�־λ 
	WWDG->CFR|=1<<9;        //ʹ����ǰ�����ж� 
} 
//������WWDG��������ֵ 
void WWDG_Set_Counter(u8 cnt) 
{ 
	WWDG->CR =(cnt&0x7F);//������7λ������ 
} 
//���ڿ��Ź��жϷ������ 
void WWDG_IRQHandler(void) 
{      
	WWDG_Set_Counter(WWDG_CNT);//���贰�ڿ��Ź���ֵ!         
	WWDG->SR=0X00;//�����ǰ�����жϱ�־λ 
	LED1=!LED1; 
}

