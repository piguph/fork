#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "touch.h"
#include "font.h"
#include "get_baidu.h"

pthread_cond_t cond;	//条件变量
pthread_mutex_t lock;	//定义一把锁


int Font_w();
//int resive();
int lcd_show_bmp(const char *bmp_name,int start_x,int start_y); //图片显示的名字
int lcd_init(void);
void lcd_uinit(int fb);

int TCP_ser();
//int ts_open();
//int ts_close(int fd);
//int ts_move(int fd);


void * WeChat(void* arg);
 



/*
oid * WeChat(void* arg);
{
	int x, y;
	while(1)
	{
		
		touch_init(&x, &y);
		if(x > 72 && x < 220 && y > 200 && y< 250)
		{
			
			
			lcd_show_bmp("listen.bmp",0,0);
		
			Font_w();
			
			
		}
	}
	
}*/

void * Speech(void* arg)
{
	int x, y;
	while(1)
	{
		touch_init(&x, &y);
		
		
		if(x > 50 && x < 660 && y > 333 && y< 450)
		{
			lcd_show_bmp("listen.bmp",0,0);
			pthread_cond_signal(&cond);

		}
		
		
	}
	
}

/*void * Weather(void* arg)
{
	int x, y;
	while(1)
	{
		touch_init(&x, &y);
		if(x > 530 && x < 660 && y > 200 && y< 250)
		{
			lcd_show_bmp("weather.bmp",0,0);
			wea();
			
		}
		
		
	}
	
}*/


int main()
{
	
	//初始化锁
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	//int ts_fd=ts_open();	
	int lcd_fb=lcd_init();
	
	//int a = ts_move(ts_fd);
	//int i=1;
	lcd_show_bmp("menu.bmp",0,0); //主界面
	

	
	//连接
	//connect();
	
	pthread_t tid1;
	pthread_create(&tid1, NULL, WeChat, NULL);
	
	
	pthread_t tid2;
	pthread_create(&tid2, NULL, Speech, NULL);
	
	/*pthread_t tid3;
	pthread_create(&tid3, NULL, Weather, NULL);*/
	
	
	
	while(1)
	{
		
		
	}
	//ts_close(ts_fd);
	lcd_uinit(lcd_fb);
	return 0;
}