#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
#include<algorithm>
#include<time.h>
#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;

void main()
{
    WSADATA wsd;
    SOCKET sockClient;                                            //客户端socket
    SOCKADDR_IN addrSrv;
    char recvBuf[3000];
	string word;
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        printf("start up failed!\n");
        return ;
    }
    sockClient=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);                    //创建socket

    addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");        //ip地址
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(6000);

	cout<<"**************************\n客户端版本：3.0\n编写人员：边维东\n**************************\n";
	//输入文件名
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //连接服务器端

	cout<<"请输入文件名："<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //向服务器端发送数据
	closesocket(sockClient);                                    //关闭连接
	cout<<"\n程序正在初始化。。。请耐心等待"<<endl;

	while(1){

	//输入选项
	sockClient=socket(AF_INET,SOCK_STREAM,0);                    //创建socket
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //连接服务器端
	cout<<"输入1：输出出现词频TOP100的单词\n输入2：输入单词，输出出现的行数及在此行出现的次数\n输入3：退出程序\n"<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //向服务器端发送数据
	closesocket(sockClient);                                    //关闭连接

	if(word=="2"){
	//输入要查询单词
	sockClient=socket(AF_INET,SOCK_STREAM,0);                    //创建socket
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //连接服务器端
	cout<<"请输入需要查询的单词："<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //向服务器端发送数据
	closesocket(sockClient);                                    //关闭连接
	}

	strcpy(recvBuf,"0");
	while(strcmp(recvBuf,"-1")!=0){
	    sockClient=socket(AF_INET,SOCK_STREAM,0);                    //创建socket
	    connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //连接服务器端
        recv(sockClient,recvBuf,3000,0);                                //接收服务器端数据
		if(strcmp(recvBuf,"-1")!=0)
			printf("%s\n",recvBuf);
	    closesocket(sockClient);                                    //关闭连接

	}
    closesocket(sockClient);                                    //关闭连接
	}
    WSACleanup();
}