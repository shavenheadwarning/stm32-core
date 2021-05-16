#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "usmart.h"
#include "stmflash.h"   

/************************************************
 ALIENTEK NANO STM32������ʵ��23
 FLASHģ��EEPROMʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 	//���鳤��
#define FLASH_SAVE_ADDR  0X08010000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
    Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,115200);	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ��
    usmart_dev.init(72);	//��ʼ��USMART	 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	KEY_Init();				//������ʼ�� 	
    printf("NANO STM32\r\n");	 
    printf("FLASH EEPROM TEST\r\n");
	printf("WK_UP:Write  KEY1:Read\r\n"); 
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//WK_UP����,д��STM32 FLASH
		{
			LED2=0;
			printf("\r\nStart Write FLASH....\r\n");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
			printf("FLASH Write Finished!\r\n");//��ʾ�������
			LED2=1;
			
		}
		if(key==KEY1_PRES)//KEY1����,��ȡ�ַ�������ʾ
		{
		    LED2=0;
			printf("\r\nStart Read FLASH.... \r\n");
			STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
			printf("The Data Readed Is:  \r\n");//��ʾ�������
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

