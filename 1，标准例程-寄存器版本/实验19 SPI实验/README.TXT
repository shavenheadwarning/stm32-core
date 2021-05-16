实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32 SPI接口的使用,驱动W25Q16实现SPI FLASH数据读写.
	
硬件资源:
	1,DS0(连接在PC0) 
	2,串口1(波特率:115200)
	3,按键KEY1(PC9)/KEY_UP(PA0,也称之为WK_UP) 
	5,W25Q16(SPI FLASH芯片,连接在SPI2上)
		
实验现象:
	本实验通过KEY_UP按键来控制W25Q16的写入，通过另外一个按键KEY1来控制W25Q16的读取。将数据打印到串口助手上。
        DS0提示程序正在运行。同时，我们可以通过USMART控制读取W25QXX的ID或者整片擦除。
	
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