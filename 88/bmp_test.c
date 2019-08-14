#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void Lcd_show_point(int x,int y,int color);
int lcd_show_bmp(const char *bmp_name,int start_x,int start_y) //图片显示的名字
{
	int bmp_fd=0;
	int bmp_width=0;
	int bmp_height=0;
	int bmp_type;
	char buf[54]={0};

	//step1：打开位图资源，权限可读可写	
	bmp_fd=open(bmp_name,O_RDWR);
	if(bmp_fd == -1)
	{
	   perror("open bmp error");
	   return -1;	
	}
	
	//step2：获取图像文件的信息
	//读取位图头部信息 
	read(bmp_fd,buf,54);
	
	//宽度 
	bmp_width =(buf[18]&0xff) |(buf[19]&0xff) <<8;
	printf("bmp_width=%d\r\n",bmp_width);
	

	//高度 
	bmp_height =(buf[22]&0xff) |(buf[23]&0xff) <<8; 
	printf("bmp_height=%d\r\n",bmp_height);	
	
	//位深
	bmp_type =(buf[28]&0xff) |(buf[29]&0xff) <<8;
	printf("bmp_type=%d\r\n",bmp_type);	

	//step3：读取文件中的RGB数据
	//RGB的大小是多少?	bmp_width*bmp_height*bmp_type/8 //不够通用
	//111*100的24位图大小	==33654 -54	= 33600（实际大小），理论大小33300（宽*高*3），差了300/3/100=1
	//宽度并不是字节对齐	112/4能整除吗？
	
	char *bmp_buf=(char *)malloc(bmp_width*bmp_height*bmp_type/8);	
	read(bmp_fd,bmp_buf,bmp_width*bmp_height*bmp_type/8);
		
	//step4：将RGB数据转换为ARGB数据 假设成功
	//将3个8bit的RGB，合并为一个32bit的ARGB
	int color=0;
	char a=0,r,g,b;

	int x,y,i=0;
	for(y=0;y<480;y++)
	{	//0起点 480终点
		for(x=0;x<800;x++,i+=3)		//0起点 800终点
		{
			b=bmp_buf[i+0];	
			g=bmp_buf[i+1];	
			r=bmp_buf[i+2];	

			color = b | g<<8 | r<<16 | a<<24;	//合并一个像素点
			Lcd_show_point(x,479-y,color);
		}
	}	
	close(bmp_fd);
	return 0;
}