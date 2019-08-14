#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>	




int *plcd = NULL;//指针的类型，代表移动时的步长
							//char 移动一次是一个颜色单位
							//int 	移动一次是一个像素点单位
			
void Lcd_show_point(int x,int y,int color)
{
	plcd[y*800+x] = color;//像素点的值32bit
}		

void lcd_clean(int color)
{
	int x=0,y=0;
	for(y=0;y<480;y++)
		for(x=0;x<800;x++)
			Lcd_show_point(x,y,color);
}

int lcd_init(void)
{
	int fb;
	/*step 1: 打开设备*/
	fb = open("/dev/fb0", O_RDWR);
	if (fb < 0)
	{
		perror("open failed:");
		return -1;
	}

	/*step 2: 获取屏幕信息==>分辨率，偏移量	*/	
	struct fb_var_screeninfo fbinfo;
	ioctl(fb, FBIOGET_VSCREENINFO, &fbinfo);
	printf("%d * %d  * %d\n", fbinfo.xres, fbinfo.yres, fbinfo.bits_per_pixel / 8);
																			
	
	/*step 3: mmap*/
	plcd = mmap(NULL, fbinfo.xres * fbinfo.yres *(fbinfo.bits_per_pixel/8), PROT_WRITE, MAP_SHARED, fb, 0);
	if (plcd == MAP_FAILED)
	{
		perror("mmap failed:"); 
		return -1;
	}

	return fb;
}

void lcd_uinit(int fb)
{
	/*step 5: munmap*/
	munmap(plcd,800*480*4);
	/*step 6: close*/
	close(fb);
}

