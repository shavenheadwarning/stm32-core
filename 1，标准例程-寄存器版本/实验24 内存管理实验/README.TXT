实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习内存管理知识,方便大工程应用.
	
硬件资源:
	1,DS0(连接在PC0) 
	2,串口1(波特率:9600)
	3,按键KEY0(PC8)/KEY1(PC9)/KEY2(PD2)  
		
实验现象:
	本实验开机后，串口输出显示提示信息，等待外部输入。KEY0用于申请内存，每次申请2K字节内存。
	KEY2用于释放申请的内存。KEY1可以向申请到的内存里面写入数据，内存申请、释放、写数据的相关信息，
        打印在串口助手上。DS0用于指示程序运行状态。本章我们还可以通过USMART调试，测试内存管理函数。 
 
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