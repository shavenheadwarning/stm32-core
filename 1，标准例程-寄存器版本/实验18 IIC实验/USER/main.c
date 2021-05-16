#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "usmart.h"
#include "24cxx.h" 
#include "myiic.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��18
 IICʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   	
//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"NANO STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��		 	
	AT24CXX_Init();			//IIC��ʼ�� 
    usmart_dev.init(72);	//��ʼ��USMART
	printf("NANO STM32\r\n");	
    printf("IIC TEST\r\n"); 	
 	while(AT24CXX_Check())//��ⲻ��24c02
	{
		printf("24C02 Check Failed!\r\n");
		delay_ms(500);
		printf("Please Check!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	printf("24C02 Ready!\r\n");
	printf("WK_UP:Write  KEY1:Read\r\n");//��ʾ��ʾ��Ϣ	  
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//WK_UP ����,д��24C02
		{
			LED2=0;
		    printf("\r\nStart Write 24C02....\r\n");
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			printf("24C02 Write Finished!\r\n");//��ʾ�������
			LED2=1;
		}
		if(key==KEY1_PRES)//KEY1 ����,��ȡ�ַ�������ʾ
		{
			LED2=0;
			printf("\r\nStart Read 24C02....\r\n");
			AT24CXX_Read(0,datatemp,SIZE);
			printf("The Data Readed Is:\r\n");//��ʾ�������
			printf("%s\r\n",datatemp);//��ʾ�������ַ���
			LED2=1;
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
