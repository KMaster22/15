#include "sonic.h"

sbit Trig = P1^0;
sbit RX = P1^1;


void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//先不启动
}


void Sendwave()
{
	char i ;
	for(i =0 ;i<8;i++)
	{
		Trig = 1;
		Delay13us();
		Trig = 0;
		Delay13us();
	}
}


//float GetDistance()
//{
//	float distance = 0;
//	
//	Sendwave();
//	TR0 = 1;
//	while((RX==1)&&(TF0==0));
//	TR0 = 0;	
//	
//	distance = ((TH0<<8)|TL0)*0.017;//cm
//	
//	TF0 = 0;
//	TL0 = 0x00;		//设置定时初值
//	TH0 = 0x00;		//设置定时初值
//	
//	return distance;
//}

u16 GetDistance()
{
	float distance = 0;
	
	TL0 = 0x00;		
	TH0 = 0x00;	
	
	Sendwave();
	TR0 = 1;
	while(RX==1&&TF0==0);
	TR0 = 0;	
	if(TF0==1){
		TF0 = 0;
	}else{
		distance = ((TH0<<8)|TL0)*0.017;//cm
	}
	
	return distance;
}
