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
    SOCKET sockClient;                                            //�ͻ���socket
    SOCKADDR_IN addrSrv;
    char recvBuf[3000];
	string word;
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        printf("start up failed!\n");
        return ;
    }
    sockClient=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);                    //����socket

    addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");        //ip��ַ
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(6000);

	cout<<"**************************\n�ͻ��˰汾��3.0\n��д��Ա����ά��\n**************************\n";
	//�����ļ���
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //���ӷ�������

	cout<<"�������ļ�����"<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //��������˷�������
	closesocket(sockClient);                                    //�ر�����
	cout<<"\n�������ڳ�ʼ�������������ĵȴ�"<<endl;

	while(1){

	//����ѡ��
	sockClient=socket(AF_INET,SOCK_STREAM,0);                    //����socket
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //���ӷ�������
	cout<<"����1��������ִ�ƵTOP100�ĵ���\n����2�����뵥�ʣ�������ֵ��������ڴ��г��ֵĴ���\n����3���˳�����\n"<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //��������˷�������
	closesocket(sockClient);                                    //�ر�����

	if(word=="2"){
	//����Ҫ��ѯ����
	sockClient=socket(AF_INET,SOCK_STREAM,0);                    //����socket
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //���ӷ�������
	cout<<"��������Ҫ��ѯ�ĵ��ʣ�"<<endl;
	cin>>word;
	send(sockClient,word.c_str(),strlen(word.c_str())+1,0);    //��������˷�������
	closesocket(sockClient);                                    //�ر�����
	}

	strcpy(recvBuf,"0");
	while(strcmp(recvBuf,"-1")!=0){
	    sockClient=socket(AF_INET,SOCK_STREAM,0);                    //����socket
	    connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //���ӷ�������
        recv(sockClient,recvBuf,3000,0);                                //���շ�����������
		if(strcmp(recvBuf,"-1")!=0)
			printf("%s\n",recvBuf);
	    closesocket(sockClient);                                    //�ر�����

	}
    closesocket(sockClient);                                    //�ر�����
	}
    WSACleanup();
}