#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��9
 PWM���ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 {	
	u16 led6pwmval=0;    
	u8 dir=1;	
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM3_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led6pwmval++;
		else led6pwmval--;	 
 		if(led6pwmval>300)dir=0;
		if(led6pwmval==0)dir=1;	   					 
		LED6_PWM_VAL=led6pwmval;			
	} 
}

