#include "sys.h"
#include "usart.h"		
#include "delay.h"	 
int main(void)
{				 
	u8 t=0;
	Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);	  		//��ʱ��ʼ��
	uart_init(72,115200); 	//���ڳ�ʼ��Ϊ115200
	while(1)
	{
		printf("t:%d\r\n",t);
		delay_ms(500);
		t++;
	}	 
}
