#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "spi.h"
#include "usmart.h"
#include "w25qxx.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��19
 SPIʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   	
//Ҫд�뵽W25Q16���ַ�������
const u8 TEXT_Buffer[]={"NANOSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,115200);  //���ڳ�ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��		 	
	W25QXX_Init();  		//SPI FLASH ��ʼ�� 
    usmart_dev.init(72);	//��ʼ��USMART	
    printf("NANO STM32\r\n");	 
    printf("SPI TEST\r\n");
	while(W25QXX_ReadID()!=W25Q16)//���W25Q16
	{
		printf("W25Q16 Check Failed!\r\n");
		delay_ms(500);
		printf("Please Check!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
    printf("W25Q16 Ready!\r\n");
	printf("KEY_UP:Write  KEY1:Read\r\n");
	FLASH_SIZE=2*1024*1024;	//FLASH ��СΪ2M�ֽ�
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP ����,д��W25Q16
		{
			LED2=0;
	        printf("\r\nStart Write W25Q16....\r\n");
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
			printf("W25Q16 Write Finished!\r\n");//��ʾ�������
			LED2=1;
		}
		if(key==KEY1_PRES)	//KEY1 ����,��ȡ�ַ�������ʾ
		{
			LED2=0;
			printf("\r\nStart Read W25Q16....\r\n");
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);				//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
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

