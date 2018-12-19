#include "stm32f10x.h" 
#include "hx711.h" 
#include "delay.h" 


#define ADIO GPIOC        //ad口  
#define DATA GPIO_Pin_11	  //数据口 
#define CLK  GPIO_Pin_12	  //时钟 
#define ADCLK RCC_APB2Periph_GPIOC  //生命ad时钟挂在APB2P.C上
 extern double canshu;
 double hx711_buffer=0;//定义hx711_buffer来接受hx711read读取的信息
 double	weight_maopi=0;//毛皮重量
 double weight_shiwu=0;//实物重量
 u32 am;//毛皮重量整数部分
  double a;//处理毛皮重量等同于毛皮重量（可有可无）
  double b;//毛皮重量小数部分
  const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};//可有可无
  #define SIZE sizeof(TEXT_Buffer)	//只是为了得到SIZE
void ADInit(void) //AD初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructurer; //结构体

	RCC_APB2PeriphClockCmd(ADCLK,ENABLE); //使能时钟口

	GPIO_InitStructurer.GPIO_Pin = CLK; //PC10为时钟口
	GPIO_InitStructurer.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出 
	GPIO_InitStructurer.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructurer); //PC.10时钟口

	GPIO_InitStructurer.GPIO_Pin = DATA; //PC11为数据口
	GPIO_InitStructurer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输出 
	GPIO_InitStructurer.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructurer);	//PC.11数据口
	} 

unsigned long HX711_Read(void)  //读取711读到的数据
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 
	float p;
	GPIO_SetBits(ADIO,DATA); 
	GPIO_ResetBits(ADIO,CLK); 
	while(GPIO_ReadInputDataBit(ADIO,DATA)); 
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(ADIO,CLK); 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(ADIO,CLK); 
		if(GPIO_ReadInputDataBit(ADIO,DATA)) 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(ADIO,CLK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(ADIO,CLK); 
	delay_us(1); 
	p=val;
	return p; 	
}
double Get_Maopi(void)
  {
	  hx711_buffer=HX711_Read();
	  weight_maopi=hx711_buffer/100;
	  weight_maopi=weight_maopi/canshu;//参数可换，不同的称重模块对应不同的参数，需自己标定
	  a=weight_maopi;
	 
     return a;	//返回值	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/canshu;//处理实物重量

	m=hx711_buffer;
	return m;
}


