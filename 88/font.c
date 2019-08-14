
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "font.h"
#include "get_baidu.h"


//初始化Lcd

int sockfd;
//char* get_baidu();
//int Send();
int k = 0;
//初始化Lcd
struct LcdDevice *init_lcd(const char *device)
{
	//申请空间
	struct LcdDevice* lcd = malloc(sizeof(struct LcdDevice));
	if(lcd == NULL)
	{
		return NULL;
	} 

	//1打开设备
	lcd->fd = open(device, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open lcd fail");
		free(lcd);
		return NULL;
	}
	
	//映射
	lcd->mp = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd->fd,0);

	return lcd;
}



int Font_w(char *buf)
{
	
	//初始化Lcd
	struct LcdDevice* lcd = init_lcd("/dev/fb0");
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	

	

		
		//字体大小的设置
		fontSetSize(f,50);
		//创建一个画板（点阵图）
		bitmap *bm = createBitmapWithInit(600,50, 4,getColor(0,150,166,162)); //也可使用createBitmapWithInit函数，改变画板颜色
		//bitmap *bm = createBitmap(288, 100, 4);			 //A  B  G R 
		//char buf[1024] = str1;
			//将字体写到点阵图上
		fontPrint(f,bm,50,0,buf,getColor(0,0,0,255),0);
									  //A  B  G R 	
									  
		//把字体框输出到LCD屏幕上
		
		show_font_to_lcd(lcd->mp,0,120,bm);
		
		
		//write(sockfd,str1, strlen(str1));
		
		//system("tftp 192.168.5.160 -g -r tts_sample.wav  ");

			//播放语音   
		//system("mplayer ./tts_sample.wav");

		//read_msg ();
		
			//关闭字体，关闭画板
		fontUnload(f);
		destroyBitmap(bm);	
		

	
		
								 
	  return 0;
	
}
