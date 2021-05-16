实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32内部FLASH模拟成EEPROM的使用方法.
	
硬件资源:
	1,DS0(连接在PC0)、DS2(连接在PC2) 
	2,串口1(波特率:115200)
	3,按键KEY1(PC9)/KEY_UP(PA0,也称之为WK_UP)  
		
实验现象:
	本实验开机后，串口输出显示提示信息，等待外部输入。KEY_UP用来执行写入FLASH的操作，
        KEY1用来执行读出FLASH的操作，读写的相关信息，打印在串口助手上。DS0用于指示程序运行状态。
        本章我们还可以通过USMART调试，不过切记写入地址不要把用户代码去给写了，否则可能死机。 
 
注意事项:  无 
	 

					正点原子@ALIENTEK
					2018-3-28
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com