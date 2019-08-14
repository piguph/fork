#include <stdio.h>
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



#include "CJSON.h"   //JSON 接口头文件   

int get_weather(char *msg,char *send_message)
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
    seraddr.sin_addr.s_addr = inet_addr("114.215.88.227");  //服务器的IP地址信息

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
	
	long cityid = 0;
	
	if(strstr(msg,"广州"))
	{
		cityid = 101280101;
	}
	if(strstr(msg,"深圳"))
	{
		cityid = 101280601;
	}
	if(strstr(msg,"北京"))
	{
		cityid = 101010100;
	}
	
	//char  *http = "GET /api/?version=v6&cityid=101280601 HTTP/1.1\r\nHost:tianqiapi.com\r\n\r\n";  
	char http[1024] = {0};
	sprintf(http,"GET /api/?version=v6&cityid=%ld HTTP/1.1\r\nHost:tianqiapi.com\r\n\r\n",cityid);
	
	
	//发送HTTP协议给百度服务器  
	write(sockfd,http,strlen(http));
	
	
	
	//读取服务器返回的数据 
	char buf[4096*10]={0};
	printf("等待服务器应答\n");
	read(sockfd,buf,4096*10);   
	

//	printf("服务器应答数据为 %s\n",buf);
	
	//找到JSON 对象  
	//printf("buf=%s\n",strstr(buf,"{"));
	//char buf1[4096*10]={0};

	
	cJSON *obj=cJSON_Parse(strstr(buf,"{")); 
		  if(obj == NULL)
		  {
			  perror("Parse fail:");
			  return -1;
		  }
		  
  
		  
		  
	 cJSON *value = cJSON_GetObjectItem(obj,"week");	
	char  *week=cJSON_Print(value);
	week = strtok(week,"\"");
	printf("week=%s\n",week);
	
	
	value = cJSON_GetObjectItem(obj,"date");	
	char  *date=cJSON_Print(value);
	date = strtok(date,"\"");
	printf("date=%s\n",date);
	
	value = cJSON_GetObjectItem(obj,"city");	
	char  *city=cJSON_Print(value);
	city = strtok(city,"\"");
	printf("city=%s\n",city);
	
	value = cJSON_GetObjectItem(obj,"wea");	
	char  *wea=cJSON_Print(value);
	wea = strtok(wea,"\"");
	printf("wea=%s\n",wea);
	
	value = cJSON_GetObjectItem(obj,"tem1");	
	char  *tem1=cJSON_Print(value);
	tem1 = strtok(tem1,"\"");
	printf("tem1=%s\n",tem1);
	
	value = cJSON_GetObjectItem(obj,"tem2");	
	char  *tem2=cJSON_Print(value);
	tem2 = strtok(tem2,"\"");
	printf("tem2=%s\n",tem2);
	
	value = cJSON_GetObjectItem(obj,"air_level");	
	char  *air_level=cJSON_Print(value);
	air_level = strtok(air_level,"\"");
	printf("air_level=%s\n",air_level);
	
	value = cJSON_GetObjectItem(obj,"air_tips");	
	char  *air_tips=cJSON_Print(value);
	air_tips = strtok(air_tips,"\"");
	printf("air_tips=%s\n",air_tips);
	
	value = cJSON_GetObjectItem(obj,"update_time");	
	char  *update_time=cJSON_Print(value);
	update_time = strtok(update_time,"\"");
	printf("update_time=%s\n",update_time);
	
	char tmp[1024] = {0};
	sprintf(tmp,"%s天气：%s,气温%s度到%s度",city,wea,tem2,tem1);
	strcpy(send_message,tmp);
	
	close(sockfd);

}
