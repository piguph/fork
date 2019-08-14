#include "touch.h"

int touch_init(int *x, int *y)
{
	int touch_fd = open("/dev/input/event0", O_RDWR);
	if ( -1 == touch_fd )
	{
		perror("open /dev/input/event0 Error ");
		return -1 ;
	}

	struct input_event buf;
	int count = 0;
	
	while(1)
	{
		read(touch_fd, &buf, sizeof(buf));  //读取一次 只能获取一个数据出来而已 
		
		//处理事件值
		if(buf.type == EV_ABS && buf.code == ABS_X)
		{
			*x = buf.value;
			count++;
		}
		if(buf.type == EV_ABS && buf.code == ABS_Y)
		{
		
			*y = buf.value;
			count++;
		}
		if (count == 2)
		{
			//printf("x = %d; y = %d\n", *x, *y);
			break;
		}
		
		
	}

	close(touch_fd);
	return 0;

}