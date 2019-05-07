#include<stdio.h>
#include<netinet/in.h> //定义数据结构sockaddr_in
#include<sys/socket.h> //提供socket函数及数据结构
#include<sys/types.h> //数据类型定义
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
int main(int argc, char *argv[])
{
	struct sockaddr_in clientaddr;//定义地址结构
	pid_t pid;
	int clientfd,sendbytes,recvbytes;//定义客户端套接字
	struct hostent *host;
	char *buf,*buf_r;
	if(argc < 4)
	{
		printf("usage:\n");
		printf("%s host port name\n",argv[0]);
		exit(1);
	}
	host = gethostbyname(argv[1]);
	if((clientfd = socket(AF_INET,SOCK_STREAM,0)) == -1) //创建客户端套接字
	{
		perror("socket\n");
		exit(1);
	}
	//绑定客户端套接字
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons((uint16_t)atoi(argv[2]));
	clientaddr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(clientaddr.sin_zero),0);
	if(connect(clientfd,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr)) == -1) //连接服务端
	{
		perror("connect\n");
		exit(1);
	}
	buf=(char *)malloc(120);
	memset(buf,0,120);
	buf_r=(char *)malloc(100);

	if( recv(clientfd,buf,100,0) == -1)
	{
		perror("recv:");
		exit(1);
	}
	printf("\n%s\n",buf);

	pid = fork();//创建子进程
	while(1)
	{
		if(pid > 0){
			//父进程用于发送信息

			//get_cur_time(time_str);

			strcpy(buf,argv[3]);
			strcat(buf,":");
			memset(buf_r,0,100);
			//gets(buf_r);
			fgets(buf_r,100,stdin);
			strncat(buf,buf_r,strlen(buf_r)-1);
			//strcat(buf,time_str);
			//printf("---%s\n",buf);
			if((sendbytes = send(clientfd,buf,strlen(buf),0)) == -1)
			{
			perror("send\n");
			exit(1);
			}
		}
		else if(pid == 0)
		{
			//子进程用于接收信息
			memset(buf,0,100);
			if(recv(clientfd,buf,100,0) <= 0)
			{
				perror("recv:");
				close(clientfd);
				raise(SIGSTOP);
				exit(1);
			}
			printf("%s\n",buf);
		}
		else
			perror("fork");
	}
	close(clientfd);
	return 0;
}