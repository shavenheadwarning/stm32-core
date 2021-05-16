#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "dma.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��17
 DMAʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   	
const u8 TEXT_TO_SEND[]={"ALIENTEK NANO STM32 DMA ����ʵ��"};
#define TEXT_LENTH  sizeof(TEXT_TO_SEND)-1			//TEXT_TO_SEND�ַ�������(������������)
u8 SendBuff[(TEXT_LENTH+2)*100];

int main(void)
{ 
	u16 i;
	u8 t=0; 

    Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��	
 	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,(TEXT_LENTH+2)*100);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
	printf("NANO STM32\r\n"); 
	printf("DMA TEST\r\n");
	printf("KEY0:Start\r\n"); 
	//��ʾ��ʾ��Ϣ	   
	for(i=0;i<(TEXT_LENTH+2)*100;i++)//���ASCII�ַ�������
    {
		if(t>=TEXT_LENTH)//���뻻�з�
		{ 
			SendBuff[i++]=0x0d; 
			SendBuff[i]=0x0a; 
			t=0;
		}else SendBuff[i]=TEXT_TO_SEND[t++];//����TEXT_TO_SEND���    
    }		  
	i=0;
	while(1)
	{
		t=KEY_Scan(0);
		if(t==KEY0_PRES)//KEY0����
		{
			printf("\r\nDMA DATA:\r\n "); 	    
		    USART1->CR3=1<<7;           //ʹ�ܴ���1��DMA����     
			MYDMA_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡	  
		    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
		    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
		    while(1)
		    {
				if(DMA1->ISR&(1<<13))//�ȴ�ͨ��4�������
				{
					DMA1->IFCR|=1<<13;//���ͨ��4������ɱ�־
					break; 
		        }
                LED2=!LED2;
                delay_ms(50);				
		    }
            LED2=1;			
			printf("Transimit Finished!\r\n");//��ʾ�������
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}
}
 



