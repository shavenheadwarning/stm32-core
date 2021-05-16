实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32的USB功能使用,实现一个简单的USB读卡器
	
硬件资源:
	1,DS0(连接在PC0) 
	2,串口1(波特率:115200)
	3,USB_SLAVE接口
	
实验现象:
	本实验开机的时候先检测SPI FLASH是否存在，如果存在则获取其容量，并打印在串口助手上。
        之后开始USB配置，在配置成功之后就可以在电脑上发现一个可移动磁盘。我们用DS1来指示
        USB正在读写SPI FLASH，信息在串口助手上打印出来，同样我们还是用DS0来指示程序正在运行。
	
注意事项:  
	1,USB线，请务必插到开发板的USB_SLAVR端口（左侧的USB口）
	 

					正点原子@ALIENTEK
					2018-3-28
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					