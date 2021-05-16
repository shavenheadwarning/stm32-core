#include "tpad.h"
#include "delay.h"		    
#include "usart.h"

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

#define TPAD_ARR_MAX_VAL 	0XFFFF	//����ARRֵ
vu16 tpad_default_val=0;//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��

//��ʼ����������
//��ÿ��ص�ʱ����������ȡֵ.
//psc:����ϵ��(ֵԽС,Խ����,��СֵΪ:1)
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
u8 TPAD_Init(u8 psc)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM3_CH4_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);//��1Mhz��Ƶ�ʼ��� 
	for(i=0;i<10;i++)//������ȡ10��
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//����
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//ȡ�м��6�����ݽ���ƽ��
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;//��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!
	return 0;		     	    					   
}
//��λһ��
void TPAD_Reset(void)
{
	
	GPIOB->CRL&=0XFFFFFF0F;	//PB1  
	GPIOB->CRL|=0X00000030;	//�������       
	GPIOB->ODR&=~(1<<1);	//���0,�ŵ�
	delay_ms(5);
	TIM3->SR=0;   			//������
	TIM3->CNT=0;			//����     
	GPIOB->CRL&=0XFFFFFF0F;	//PB1 ����  
	GPIOB->CRL|=0X00000040;	//��������  
  
}
//�õ���ʱ������ֵ
//�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
u16 TPAD_Get_Val(void)
{				   
	
   	TPAD_Reset();
	while((TIM3->SR&0X10)==0)//�ȴ�����������(TIM3->CC4)
	{
		if(TIM3->CNT>TPAD_ARR_MAX_VAL-500)return TIM3->CNT;//��ʱ��,ֱ�ӷ���CNT��ֵ
	};	
	return TIM3->CCR4;
	
} 	 
//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//�õ�һ��ֵ
		if(temp>res)res=temp;
	};
	return res;
}  
//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;										  
#define TPAD_GATE_VAL 	30	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;		//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;		//֧������	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//����tpad_default_val+TPAD_GATE_VAL,��Ч
	{							 
		if(keyen==0)res=1;		//keyen==0,��Ч 
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	
//��ʱ��3ͨ��4���벶������
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_CH4_Cap_Init(u16 arr,u16 psc)
{

	RCC->APB1ENR|=1<<1;   	//TIM3 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ�� 
	
	GPIOB->CRL&=0XFFFFFF0F;	//PB1����  
	GPIOB->CRL|=0X00000040;	//��������

	TIM3->ARR=arr;  		//�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  		//Ԥ��Ƶ��,1M�ļ���Ƶ��
	
	TIM3->CCMR2|=1<<8;		//CC4S=01 	ѡ������� IC4ӳ�䵽TI4��
 	TIM3->CCMR2|=0<<12; 	//IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
 	TIM3->CCMR2|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM3->CCER|=0<<13; 		//CC4P=0	�����ز���
	TIM3->CCER|=1<<12; 		//CC4E=1 	�������������ֵ������Ĵ�����
																 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3
	
}
