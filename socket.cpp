#include<fstream>
#include<iostream>
#include<string>
#include<map>
#include<list>
#include<iomanip>
#include<algorithm>
#include<time.h>
#include<vector> 
#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;


class InputHandler{
public:
	int line;
    multimap<string,int> init2;
    map<string,int> init;
    vector<string> vect;
    InputHandler(string name){
        file_name = name;
    }
    void get_data();
	string file_name;
};

class base{
public:
    virtual void process(InputHandler& data){};
};

class WordCounter : public base{
public:
    multimap<int,string> CountedData;
    virtual void process(InputHandler& data);
};

void DeleteMark(string&);
//string ReadLine(InputHandler& data,int line);

void WordCounter :: process(InputHandler& data){
	int i;
    map<string,int> :: iterator start = data.init.begin();
	
    i=data.init.size();

    for(;i >= 1; i--){
        string temp = start -> first;
        if(temp != "" && temp != "--" && temp != "-" && temp != "\0"){
            DeleteMark(temp);
            CountedData.insert(make_pair(start -> second, temp));
        }
		start++;
    }
}

class WordPosRecorder : public base{
public:
	multimap<string,int> *SearchData;
	virtual void process(InputHandler& data);
};

void WordPosRecorder :: process(InputHandler& data){
	SearchData=&data.init2;
}


class OutputHandler{
public:
    void output(WordCounter& DataOut,SOCKET server,SOCKET client,SOCKADDR_IN addrClient);
	void OutputHandler :: output(WordPosRecorder& DataOut,string str,InputHandler& data,SOCKET server,SOCKET client,SOCKADDR_IN addrClient);
};


void OutputHandler :: output(WordCounter& DataOut,SOCKET server,SOCKET client,SOCKADDR_IN addrClient){
	char sendBuf[3000];
    int len=sizeof(SOCKADDR);
    multimap<int,string> :: iterator start = DataOut.CountedData.end();
    start--;
    for(int i = 1;i <= 100 && start!=DataOut.CountedData.begin();i ++){
		if( (start -> second) .length() != 0 ){
			client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
			sprintf(sendBuf,"���ʣ�%s\tƵ����%d\n", start -> second.c_str(),start -> first);
			cout << "���ʣ�" << start -> second << "\t\t" <<"Ƶ����" << start -> first << endl;
			send(client,sendBuf,strlen(sendBuf)+1,0);            //������Ϣ�ͻ���
			closesocket(client);
		}
		start--;
	}
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	sprintf(sendBuf,"���ʣ�%s\tƵ����%d\n", start -> second.c_str(),start -> first);
	cout << "���ʣ�" << start -> second << "\t\t" <<"Ƶ����" << start -> first << endl;
	send(client,sendBuf,strlen(sendBuf)+1,0);            //������Ϣ�ͻ���
	//����ֹͣ��������
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	sprintf(sendBuf,"-1");
	send(client,sendBuf,strlen(sendBuf)+1,0);            //������Ϣ�ͻ���
	closesocket(client);

}

void OutputHandler :: output(WordPosRecorder& DataOut,string str,InputHandler& data,SOCKET server,SOCKET client,SOCKADDR_IN addrClient){
	int a=0;
	multimap<string,int> :: iterator p;
	p=(*(DataOut.SearchData)).find(str);
	char sendBuf[3000];
    int len=sizeof(SOCKADDR);

	if( p!=((*(DataOut.SearchData))).end()){
	while((*p).first==str)
	{
		client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
		if(a!=(*p).second){
			a=(*p).second;
			sprintf(sendBuf,"\n�����У�\n%d\n����Ϊ��\n%s\n", (*p).second,(data.vect[(*p).second-1]).c_str());
			printf("\n�����У�\n%d\n����Ϊ��\n%s\n", (*p).second,(data.vect[(*p).second-1]).c_str());
			send(client,sendBuf,strlen(sendBuf)+1,0);            //������Ϣ�ͻ���
		}

		p++;
		if(p==(*(DataOut.SearchData)).end())
			break;

	    closesocket(client);
	 }
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	sprintf(sendBuf,"-1");
	send(client,sendBuf,strlen(sendBuf)+1,0);            //������Ϣ�ͻ���
	closesocket(client);
	}


}

void DeleteMark(string& ToDeal){
    int l,i;
	l=ToDeal.length();
	char c[100];
	strcpy(c,ToDeal.c_str());


    for(i=0;i<l;){
        if(!('a' <= c[i] && c[i] <= 'z' || 'A' <= c[i] && c[i] <= 'Z' || c[i] == '-' || c[i] == '\0' || c[i] == '\'')){
            ToDeal.erase(i,1);
			l--;
		}
		else
			i++;
    }
}

void InputHandler :: get_data(){
    ifstream Infile;
	string temp;
	line=0;
	char c[3000];
	char *p; 
    Infile.open(file_name.c_str());

    while(!Infile.eof()){

        getline(Infile,temp);
        vect.push_back(temp);
		line++;
		if(temp!=""){
			strcpy(c,temp.c_str());
		p=strtok(c," ");
		if(p!=NULL){
			string temp2(p);
			while(p!=NULL)
			{
				DeleteMark(temp2);
				init2.insert(make_pair(temp2,line));
				init[temp2] ++;
				p=strtok(NULL," ");
				if(p!=NULL)
					temp2=p;
			} 
		}
		}
	}
    Infile.close();
    Infile.clear();
}



void main()
{
	int time1; 
	int choice;
	string word;
	WordCounter counter;
	WordPosRecorder pos; 
	OutputHandler outputer;
	vector<base*>vec;
	vec.push_back(&counter);
	vec.push_back(&pos);
    WSADATA wsd;
    SOCKET server;                                            //������socket
    SOCKADDR_IN addrSrv;    
    char sendBuf[3000];
    char recvBuf[100];
    SOCKADDR_IN addrClient;
    SOCKET client;                                            //���ӵĿͻ���socket
    int len;
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        printf("start up failed!\n");
        return ;
    }
    server=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);                    //����socket
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);            //���õ�ַ
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(6000); //���ö˿ں�
    bind(server,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));        //��
    listen(server,5);                                        //�������������
    len=sizeof(SOCKADDR);


	cout<<"**************************\n����˰汾��3.0\n��д��Ա����ά��\n**************************\n";
    cout<<"�������ļ�����";
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	recv(client,recvBuf,100,0);
	string name(recvBuf);
    cout<<"\n�������ڳ�ʼ�������������ĵȴ�"<<endl;
    time1=time(NULL);
	InputHandler input(name);
	input.get_data(); 
	printf("\n��ʱ%d��\n",(time(NULL)-time1));
	
	cout<<"����1��������ִ�ƵTOP100�ĵ���\n����2�����뵥�ʣ�������ֵ��������ڴ��г��ֵĴ���\n����3���˳�����\n";
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	recv(client,recvBuf,100,0);
		
    while(strcmp(recvBuf,"1")==0 || strcmp(recvBuf,"2")==0){
    	
	if(strcmp(recvBuf,"1")==0){
    vec[0]->process(input);
	outputer.output(counter,server,client,addrClient);
	}
	
	if(strcmp(recvBuf,"2")==0){
	vec[1]->process(input); 
	cout<<"�����뵥�ʣ�"<<endl;
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	recv(client,recvBuf,100,0);
	string word(recvBuf);
	outputer.output(pos, word,input,server,client,addrClient);	
	}
	
	cout<<"����1��������ִ�ƵTOP100�ĵ���\n����2�����뵥�ʣ�������ֵ��������ڴ��г��ֵĴ���\n����3���˳�����\n";
	client=accept(server,(SOCKADDR*)&addrClient,&len);    //���տͻ�������
	recv(client,recvBuf,100,0);
	
    }
	closesocket(client);                                    //�ر�����
    WSACleanup();

	system("pause");
}