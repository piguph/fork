#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int Font_w(char *buf);

int get_weather(char *msg,char *send_message);
char main_str[1024] = {0};

char* get_baidu()
{
	
	//1.创建网络通信对象
	int sockfd = socket(AF_INET, SOCK_STREAM , 0);
	if (sockfd == -1) 
	{
		perror("socket error!");
		return 0;
	}

	//2.定义网络信息结构体
    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(80);   //所有的HTTP请求端口都是80 
    seraddr.sin_addr.s_addr = inet_addr("47.107.120.234");  //服务器的IP地址信息

	//3.链接网络服务器
	int ret = connect(sockfd,  (struct sockaddr *)&seraddr, sizeof(struct sockaddr));
	if (ret == -1)
	{
		perror("connect error");
		return 0;
	}
	else
	{
		printf("connect ok\n");
	}
	
	//www.baidu.com/ 
	//www.baidu.com  -> HOST 
	//  / 		-》 URL 
	
	//http://api.qingyunke.com/api.php?key=free&appid=0&msg=问题
	
	//HOST ： api.qingyunke.com 
	//URL  :  /api.php?key=free&appid=0&msg=问题
	//制定HTTP 请求协议  
	
		//char buf[1024] = {0};
		char buf1[1024] = {0};
		char buf2[100] = "天气";
		char http[1024] = {0};
		printf("请输入：\n");
		scanf("%s", buf1);
		while(getchar()!='\n');
	if(strstr(buf1, "天气") != 0)
	{
		bzero(main_str, 1024);
		get_weather(buf1,main_str);
		goto exit;
		
	}
	//char  *http = "GET /api.php?key=free&appid=0&msg=您贵庚呀 HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n";  
	sprintf(http , "GET /api.php?key=free&appid=0&msg=%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n", buf1);

	//发送HTTP协议给百度服务器  
	write(sockfd,http,strlen(http));
	
	
	
	//读取服务器返回的数据 
	char buf[4096*10]={0};
	
	
	printf("等待服务器应答\n");
	read(sockfd,buf,4096*10);   
	
	//去除头数据 
	char del_head[1024]={0};
	//char main_str[1024] = {0};
	char str[1024] = {0};
	//del_head = strstr(buf,"\r\n\r\n");
	char *p = strstr(buf,"\r\n\r\n");
	int i = 0, j = 0;
	int num = strlen(p) - 29;
	for(i = 27; i < num, j < num; i++, j++)
	{
		str[j] = p[i];
	}
	
	
	//printf("%s\n", str);
	strcpy(main_str,str);
	printf("%s\n",main_str);
exit:	
		Font_w(main_str);
		return main_str;


}
