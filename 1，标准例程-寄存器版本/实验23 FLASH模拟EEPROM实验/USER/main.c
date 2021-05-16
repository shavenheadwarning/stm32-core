#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "usmart.h"
#include "stmflash.h"   

/************************************************
 ALIENTEK NANO STM32开发板实验23
 FLASH模拟EEPROM实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 	//数组长度
#define FLASH_SAVE_ADDR  0X08010000 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
    Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化
    usmart_dev.init(72);	//初始化USMART	 
	LED_Init();		  		//初始化与LED连接的硬件接口
 	KEY_Init();				//按键初始化 	
    printf("NANO STM32\r\n");	 
    printf("FLASH EEPROM TEST\r\n");
	printf("WK_UP:Write  KEY1:Read\r\n"); 
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//WK_UP按下,写入STM32 FLASH
		{
			LED2=0;
			printf("\r\nStart Write FLASH....\r\n");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
			printf("FLASH Write Finished!\r\n");//提示传送完成
			LED2=1;
			
		}
		if(key==KEY1_PRES)//KEY1按下,读取字符串并显示
		{
		    LED2=0;
			printf("\r\nStart Read FLASH.... \r\n");
			STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
			printf("The Data Readed Is:  \r\n");//提示传送完成
			printf("%s\r\n",datatemp);//显示读到的字符串
			LED2=1;
		}
		i++;
		delay_ms(10);  
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}   	   
}

