#include "stm32f10x.h" 
#include "hx711.h" 
#include "delay.h" 


#define ADIO GPIOC        //ad��  
#define DATA GPIO_Pin_11	  //���ݿ� 
#define CLK  GPIO_Pin_12	  //ʱ�� 
#define ADCLK RCC_APB2Periph_GPIOC  //����adʱ�ӹ���APB2P.C��
 extern double canshu;
 double hx711_buffer=0;//����hx711_buffer������hx711read��ȡ����Ϣ
 double	weight_maopi=0;//ëƤ����
 double weight_shiwu=0;//ʵ������
 u32 am;//ëƤ������������
  double a;//����ëƤ������ͬ��ëƤ���������п��ޣ�
  double b;//ëƤ����С������
  const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};//���п���
  #define SIZE sizeof(TEXT_Buffer)	//ֻ��Ϊ�˵õ�SIZE
void ADInit(void) //AD��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructurer; //�ṹ��

	RCC_APB2PeriphClockCmd(ADCLK,ENABLE); //ʹ��ʱ�ӿ�

	GPIO_InitStructurer.GPIO_Pin = CLK; //PC10Ϊʱ�ӿ�
	GPIO_InitStructurer.GPIO_Mode = GPIO_Mode_Out_PP; //������� 
	GPIO_InitStructurer.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructurer); //PC.10ʱ�ӿ�

	GPIO_InitStructurer.GPIO_Pin = DATA; //PC11Ϊ���ݿ�
	GPIO_InitStructurer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//������� 
	GPIO_InitStructurer.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructurer);	//PC.11���ݿ�
	} 

unsigned long HX711_Read(void)  //��ȡ711����������
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
	  weight_maopi=weight_maopi/canshu;//�����ɻ�����ͬ�ĳ���ģ���Ӧ��ͬ�Ĳ��������Լ��궨
	  a=weight_maopi;
	 
     return a;	//����ֵ	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/canshu;//����ʵ������

	m=hx711_buffer;
	return m;
}


