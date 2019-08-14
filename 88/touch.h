#ifndef _TOUCH_H_
#define _TOUCH_H_

//头文件



//宏定义 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>


//函数声明 
int touch_init(int *x, int *y);

#endif