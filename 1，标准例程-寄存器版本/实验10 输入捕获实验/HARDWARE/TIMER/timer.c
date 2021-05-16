#include "timer.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  	  

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ�� 
	RCC->APB2ENR|=1<<4; //ʹ��PORTCʱ��	 
	
	GPIOC->CRL&=0XF0FFFFFF;	//PC6���
	GPIOC->CRL|=0X0B000000;	//���ù������ 	  
	
	RCC->APB2ENR|=1<<0;     //ʹ��AFIOʱ��	
    AFIO->MAPR&=0XFFFFF3FF; //���MAPR��[11:10]
	AFIO->MAPR|=3<<10;      //������ӳ��,TIM3_CH1->PC6

	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM3->CCMR1|=1<<3; 	    //CH1Ԥװ��ʹ��	   
	TIM3->CCER|=1<<0;   	//OC1 ���ʹ��	   
	TIM3->CR1=0x0080;   	//ARPEʹ�� 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 	
	
	
}

//��ʱ��2ͨ��1���벶������
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	RCC->APB1ENR|=1<<0;   	//TIM2 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��  
	 
	GPIOA->CRL&=0XFFFFFFF0;	//PA0 ���֮ǰ����  
	GPIOA->CRL|=0X00000008;	//PA0 ����   
	GPIOA->ODR|=0<<0;		//PA0 ����
	  
 	TIM2->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM2->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM2->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM2->CCMR1|=1<<4; 		//IC1F=0001 ���������˲��� ��Fck_int������2���¼�����Ч
 	TIM2->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM2->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM2->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM2->DIER|=1<<1;   	//�������ж�				
	TIM2->DIER|=1<<0;   	//��������ж�	
	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//��ռ2�������ȼ�0����2
 
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���
u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM2->SR;
 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//���
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//����1���������¼�
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			    TIM2CH1_CAPTURE_VAL=TIM2->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM2->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{ 
				TIM2CH1_CAPTURE_VAL=0;
				TIM2CH1_CAPTURE_STA=0X40;		//��ǲ�����������
				TIM2->CNT=0;					//���������
				TIM2->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز��� 
			}		    
		}			     	    					   
 	}
	TIM2->SR=0;//����жϱ�־λ 	    
}
