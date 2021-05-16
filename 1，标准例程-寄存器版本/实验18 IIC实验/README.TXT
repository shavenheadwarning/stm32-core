实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32模拟IIC的使用,驱动24C02实现EEPROM数据读写.
	
硬件资源:
	1,DS0(连接在PC0) 
	2,串口1(波特率:115200)
	3,按键KEY1(PC9)/KEY_UP(PA0,也称之为WK_UP)
	5,24C02(IIC连接在PB6/PB7上面) 
		
实验现象:
	本实验通过KEY_UP按键来控制24C02的写入，通过另外一个按键KEY1来控制24C02的读取。数据打印在串            口调试助手上。同时，我们可以通过USMART控制在24C02的任意地址写入和读取数据。
	
注意事项:  无
     
	

					正点原子@ALIENTEK
					2014-3-09
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com
					
					
					
					
					
					
					
					
					
					