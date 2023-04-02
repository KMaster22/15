#include "system.h"


#define DISTANCE 0 // 显示
#define PARAM   1  //参数
u8 display = DISTANCE;

// 超声波的参数
u16 dis = 30;


//系统初始化函数
void SystemProcess()
{
	DeviceCtrl(0xa0,0x00);//buzz relay
	DeviceCtrl(0x80,0xff); //led
}

//字符发送
void SendByte(u8 dat)
{
	SBUF = dat;
	while(!TI);
	TI =0;
}

void SendString(char *str)
{
	while(*str != '\0'){
		SendByte(*str++);
	}
	
}

//超声波测距
u8 sonicnt = 0;
u16 distance =0;

void SonicProcess()
{
	if(sonicnt >150){
		sonicnt = 0;
		distance = GetDistance();
	
		
	}
}

//按键获取函数

u8 keycnt =0 ;

void KbdProcess()
{
	char buf[28];

	if(keycnt>15){
		keycnt = 0;
		KbdThree();// 15ms 一次调用
		if(Trg==0x88){ //s4
			display = (display+1)%2;
		}
		if(Trg==0x48){ //s8
			if(display==DISTANCE){
				dis = distance;
			}
		}
		if(Trg==0x28){//s12
			if(display == PARAM){
					dis = dis+10;
			}		
		}
		if(Trg==0x18){//s16
			if(display == PARAM){
					dis = dis-10;
			}		
		}
		if(Trg==0x44){//s9
			sprintf(buf,"\"距离Distance:%d\": cm\r\n",distance); //转义写法
			SendString(buf);
			memset(buf,'\0',28);
		}
		
	}
}

//数码管缓存函数

u8 tubecnt = 0;

void SmgProcess()
{
	if(tubecnt>1){
		tubecnt = 0;
		if(display==DISTANCE){
			smgbuf[0] = 0x3E;
			smgbuf[1] = t_display[1];
			smgbuf[2] = t_display[10];
			smgbuf[3] = t_display[10];
			smgbuf[4] = t_display[10];
			smgbuf[5] = t_display[distance/100%10];
			smgbuf[6] = t_display[distance/10%10];
			smgbuf[7] = t_display[distance%10];
		}else if(display==PARAM){
			smgbuf[0] = 0x3E;
			smgbuf[1] = t_display[2];
			smgbuf[2] = t_display[10];
			smgbuf[3] = t_display[10];
			smgbuf[4] = t_display[10];
			smgbuf[5] = t_display[dis/100%10];
			smgbuf[6] = t_display[dis/10%10];
			smgbuf[7] = t_display[dis%10];
		}
	}
	
}

void main()
{
	Timer2Init();
	UartInit();
	SystemProcess();
	Timer0Init();
	while(1){
		SmgProcess();
		KbdProcess();
		SonicProcess();
	}

	
}
//led
u8 ledbit = 0xff;
bit ledflag = 0;
u16 ledcnt = 0;

void LedCtrl()
{
	if(display ==DISTANCE){
		ledbit = 0xfe;
	}	
	if(display == PARAM){
		ledbit = 0xfd;
	}
	
	if(distance>dis){
		ledcnt++;
		ledflag =1;
	}else{
		ledflag = 0;
	}
	
	if(ledflag){
		if(ledcnt<=200){
			ledbit &= 0xfb;
		}else if(ledcnt<= 400){
			ledbit |= 0x04;
		}else{
			ledcnt = 0;
		}
	}else{
		ledbit |= 0x04;
	}
	
	DeviceCtrl(0x80,ledbit); //led
	
}

void Timer2Handler() interrupt 12
{
	static u8  tube = 0 ;
	tube++;

	keycnt++;
	sonicnt++;
	tubecnt++;
	if(tube>=2){
		tube = 0;
		ScanfSmg();
	}
	LedCtrl();
	
	
}

void UartHandler()interrupt 4
{
	if(RI){
		RI =0;
		
		
		
	}
}


