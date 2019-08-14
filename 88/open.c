#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "get_baidu.h"
#include "touch.h"

//int touch_init(int *x, int *y);

extern pthread_cond_t cond;	//条件变量
extern pthread_mutex_t lock;	//定义一把锁
void start();
void fonk_chat(char buf[]);
//int chat_HTTP(int socket_fd);
//void fanhui();

int receive_wva(int new_socket,int socket_fd,char buff[])
{
	//接受客户端发送过来的数据  
	
	//读取头信息
    char head[1024]={0};    
	read(new_socket,head,1024);
	
	//if(strstr(head," "))
	//{
		//system("rm Nts_sample.wav");
		//printf("用户发送文件是否接收  1 OK  2 NO\n");
		//int a = 0; 
		//scanf("%d",&a);
		//if(a == 1)  //接收
		//{
		//	write(new_socket,"OK",strlen("OK")); 
			
			//创建新文件  
			char filename[1024]={0};
			int  filesize=0;
			sscanf(head,"%s %d",filename,&filesize);
			
			filename[0] = 'N';
			printf("%s %d\n",filename,filesize);
			//printf("6666666666666666666\n");
			int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("error:");
			}
			//开始接收文本数据  
			int len=0;
			while(1)
			{	
				char buf[1024]={0};
				int ret = read(new_socket,buf,1024);	//从客户端读传过来的内容尽buf
					len+= ret; 
					sleep(1);
					write(fd,buf,ret);		//写进新文件里面
				
					if(len == filesize)
					{
						printf("文件接收完毕\n");
						close(fd);
						break;
					}
				
			}	
		//}
		
	printf("%s\n",buff );
	//fonk_chat(buff);
	printf("开始播放语音\n");
	system("mplayer  Nts_sample.wav");
	

	//chat_HTTP(socket_fd);	//返回开始位置
	
}

void*WeChat(void* arg)
{
	int x, y;
	//1.创建网络的通信对象 
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);

	if(socket_fd < 0)
	{
		perror("creat fail:\n");
		return NULL;
	}
	printf("AAAAAAAA\n");
	//2.设置服务器注册的网络端口 与 网络IP  
	struct sockaddr_in  seraddr;  
	seraddr.sin_family 	     = 	AF_INET;//设置网络层协议 
	seraddr.sin_addr.s_addr  =  inet_addr("0.0.0.0"); //设置IP地址 
	seraddr.sin_port	     =	htons(6666); //设置端口号
	
	
	//3.绑定服务器的IP地址信息
	 int ret =bind(socket_fd,(struct sockaddr *)&seraddr,sizeof(seraddr));

	if(ret != 0)
	{
		perror("bind fail:");
		return NULL;
	}
	else
	{
		printf("bind ok\n");
	}

	
	//4.把服务器socket 设置成监听模式 
	ret = listen(socket_fd,5);
	if(ret != 0)
	{
		perror("listen fail:");
		return NULL;
	}
	else
	{
		printf("listen ok\n");
	}
	
	
	//5.接收客户端的链接请求
	printf("等待客户端的链接请求\n");
	int new_socket=accept(socket_fd,NULL,NULL);
	if(new_socket < 0)
	{
		perror("accept socket fail:");
		return NULL;
	}
	else
	{
		printf("new_socket = %d\n",new_socket);
	}
	
	
	while(1)
	{
		pthread_mutex_lock(&lock);
		
		pthread_cond_wait(&cond,&lock);
	
		char* p = NULL;
		p = get_baidu();
		//printf("%s\n", p);
		
		int i = 0, j = 0;
		char buf[1024] = {0};
		int n = strlen(p);
		for(i = 0; i < n, j < n ; i++,j++)
		{
			buf[j] = p[i];
		}
		 
		 //sleep(1);
		int ret_w=write(new_socket,buf,strlen(buf));
		printf("server:%s\n",buf );
		receive_wva(new_socket,socket_fd,buf);

		pthread_mutex_unlock(&lock);
		
		
		
	}		
		
		
		//close(socket_fd);
	
}