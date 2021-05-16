#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "smg.h"
#include "remote.h" 
#include "timer.h"
#include "beep.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��20
 ����ң��ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .��ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 key=0;  //����ֵ
u8 num=0x00;//��ֵ
u8 num1=0x00;//��ֵ
u8 smg_wei=6;//�����λѡ
u8 smg_duan=0;//����ܶ�ѡ
u8 smg_flag=0;//�������ʾ��־ 0:������ʾ 1:����ʾ��������Ӱ��
u8 t=0;
 int main(void)
 { 
	
	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,115200);	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	BEEP_Init();            //��������ʼ��
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    LED_SMG_Init();         //����ܳ�ʼ��
    TIM4_Init(19,7199);     //�����2ms��ʱ��ʾ
    Remote_Init();			//������ճ�ʼ��		 
	while(1);
}

void TIM4_IRQHandler(void) //TIM4�ж�
{
	
	if(TIM4->SR&0X0001)//����ж�
	{
        key = Remote_Scan();//��ȡ����ң�ؼ�ֵ
		 if(key)
		 {
			switch(key)
			{
				case 104:num1=0x00; num = smg_num[1]; BEEP=0;break; //����'1'
				case 152:num1=0x00;num=smg_num[2];BEEP=0;break;     //����'2'	   
				case 176:num1=0x00;num=smg_num[3];BEEP=0;break;     //����'3'	    
				case 48:num1=0x00;num=smg_num[4];BEEP=0;break;      //����'4'		    
				case 24:num1=0x00;num=smg_num[5];BEEP=0;break;      //����'5'		    
				case 122:num1=0x00;num=smg_num[6];BEEP=0;break;     //����'6'		  
				case 16:num1=0x00;num=smg_num[7];BEEP=0;break;      //����'7'			   					
				case 56:num1=0x00;num=smg_num[8];BEEP=0;break;      //����'8'	 
				case 90:num1=0x00;num=smg_num[9];BEEP=0;break;      //����'9'
				case 66:num1=0x00;num=smg_num[0];BEEP=0;break;      //����'0'
				case 82:num1=0x00;num=smg_num[17];BEEP=0;break;     //����'DELETE'
				case 162:num1=smg_num[1];num=smg_num[0];BEEP=0; break;//����'POWER'    
				case 98:num1=smg_num[1];num=smg_num[1];BEEP=0;  break;//����'UP'
				case 226:num1=smg_num[1];num=smg_num[2];BEEP=0; break;//����'ALIENTEK'
				case 34:num1=smg_num[1];num=smg_num[3];BEEP=0;  break;//����'LEFT'
				case 2:num1=smg_num[1];num=smg_num[4];BEEP=0;   break;//����'PLAY'
				case 194:num1=smg_num[1];num=smg_num[5];BEEP=0; break;//����'RIGHT'
				case 224:num1=smg_num[1];num=smg_num[6];BEEP=0; break;//����'VOL-'
				case 168:num1=smg_num[1];num=smg_num[7];BEEP=0; break;//����'DOWN'
				case 144:num1=smg_num[1];num=smg_num[8];BEEP=0; break;//����'VOL+'
			}
		 }else
		 {
			BEEP=1;
		 }
		 if(smg_wei==6)//�����λ
		 {
			 smg_duan = num1;
		 }
		 else if(smg_wei==7)//�����λ
		 {
			 smg_duan = num;
		 }
		 if(smg_flag) LED_Write_Data(0x00,smg_wei);//������Ӱ(���벻��ʾ)
		 else 	  LED_Write_Data(smg_duan,smg_wei);//������ʾ 
		 LED_Refresh();//��������ݸ���
		 smg_flag=!smg_flag;
		 if(smg_flag==0)//������ʾ�Ÿ���λ��
		 {
			 smg_wei++;
		     if(smg_wei==8) smg_wei=6;
		 }
		 t++;
		 if(t==250)//LED0ÿ500MS��˸
		 {
			t=0;
			LED0=!LED0;
		 } 
	}
	TIM4->SR&=~(1<<0);//����жϱ�־λ 
		
}
