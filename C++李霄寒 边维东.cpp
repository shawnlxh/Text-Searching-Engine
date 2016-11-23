#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<map>
#include<list>
#include<iomanip>
#include<algorithm>
#include<time.h>
#include<vector> 

using namespace std;


class InputHandler {
public:
	int line;
	multimap<string, int> init2;
	map<string, int> init;
	vector<string> vect;
	InputHandler(string name) {
		file_name = name;
	}
	void get_data();
	string file_name;
};

class base {
public:
	virtual void process(InputHandler& data) {};
};

class WordCounter : public base {
public:
	multimap<int, string> CountedData;
	virtual void process(InputHandler& data);
};

void DeleteMark(string&);
//string ReadLine(InputHandler& data,int line);

void WordCounter::process(InputHandler& data) {

	for (auto iter : data.init) {
		/*
		string temp = iter.first;
		if (temp != "" && temp != "--" && temp != "-" && temp != "\0") {
			DeleteMark(temp);
			CountedData.insert({ iter.second, temp });
		}
		*/
		CountedData.insert({ iter.second, iter.first});
	}
}

class WordPosRecorder : public base {
public:
	multimap<string, int> SearchData;
	virtual void process(InputHandler data);
};

void WordPosRecorder::process(InputHandler data) {
	//SearchData = data.init2;
}


class OutputHandler {
public:
	void output(WordCounter& DataOut);
	void output(string str, InputHandler& data);
};


void OutputHandler::output(WordCounter& DataOut) {
	multimap<int, string> ::iterator start = DataOut.CountedData.end();
	start--;
	for (int i = 1; i <= 100 && start != DataOut.CountedData.begin(); i++) {
		if ((start->second).length() != 0)
			cout << "单词：" << start->second << "\t\t" << "频数：" << start->first << endl;
		start--;
	}
	cout << "单词：" << start->second << "\t\t" << "频数：" << start->first << endl;

}

void OutputHandler::output(string str, InputHandler& data) {
	auto p = data.init2.find(str);
	while (p->first == str)
	{
		cout << "所在行：" << p->second << endl;
		cout << data.vect[p->second - 1] << endl;
		++p;
	}

}

void DeleteMark(string& ToDeal) {
	int l, i;
	l = ToDeal.length();
	/*
	char c[100];
	strcpy(c,ToDeal.c_str());
	*/

	for (i = 0; i < l;) {
		char ch = ToDeal[i];
		if (!('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '-' || ch == '\0' || ch == '\'')) {
			ToDeal.erase(i, 1);
			l--;
		}
		else
			i++;
	}
}

void InputHandler::get_data() {
	ifstream Infile;
	string temp;
	line = 0;
	Infile.open(file_name.c_str());
	while (!Infile.eof()) {

		getline(Infile, temp);
		istringstream is(temp);
		vect.push_back(temp);
		line++;
		/*
		char *p;
		if(temp!=""){
		char c[3000];
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
		*/
		string temp2;
		while (is >> temp2)
		{
			DeleteMark(temp2);
			if (temp2 != "") {
				init2.insert({ temp2,line });
				init[temp2]++;
			}
		}
	}
	Infile.close();
	Infile.clear();
}


int main() {
	int time1;
	int choice;
	string name;
	string word;

	OutputHandler outputer;
	base* itf;
	WordCounter counter;
	WordPosRecorder pos;
	/*
	WordCounter counter;
	WordPosRecorder pos;
	vector<base*>vec;
	vec.push_back(&counter);
	vec.push_back(&pos);
	*/
	cout << "**************************\n版本：2.0\n编写人员：李霄寒 边维东\n**************************\n";
	cout << "请输入文件名：";
	std::cin >> name;
	cout << "程序正在初始化。。。请耐心等待";
	time1 = time(NULL);
	InputHandler input(name);
	input.get_data();
	printf("\n用时%d秒\n", (time(NULL) - time1));

	cout << "输入1：输出出现词频TOP100的单词\n输入2：输入单词，输出出现的行数及在此行出现的次数\n输入3：退出程序\n";
	std::cin >> choice;

	while (choice == 1 || choice == 2) {

		if (choice == 1) {
			itf = new WordCounter;
			counter.process(input);
			outputer.output(counter);
		}

		if (choice == 2) {
			itf = new WordPosRecorder;
			cout << "请输入单词：" << endl;
			std::cin >> word;
			//pos.process(input);
			outputer.output(word, input);
		}

		cout << "输入1：输出出现词频TOP100的单词\n输入2：输入单词，输出出现的行数及在此行出现的次数\n输入3：退出程序\n";
		std::cin >> choice;

	}
	system("pause");
	return 0;
}


