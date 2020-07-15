// bean_plugin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<stdio.h>
#include<stdlib.h>
#include <map>

using namespace std;

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>


int startsWith(string s, string sub) {
	return s.find(sub) == 0 ? 1 : 0;
}

int endsWith(string s, string sub) {
	return s.rfind(sub) == (s.length() - sub.length()) ? 1 : 0;
}


std::vector<std::string> split(const char *s, const char *delim)
{
	std::vector<std::string> result;
	if (s && strlen(s))
	{
		int len = strlen(s);
		char *src = new char[len + 1];
		strcpy(src, s);
		src[len] = '\0';
		char *tokenptr = strtok(src, delim);
		while (tokenptr != NULL)
		{
			std::string tk = tokenptr;
			result.emplace_back(tk);
			tokenptr = strtok(NULL, delim);
		}
		delete[] src;
	}
	return result;
}
void readGcode(char *fileName,char *dirPath)
{
	char buf[1024];
	char nameBuf[1024];
	ifstream infile;
	ofstream ofile;               //定义输出文件
	infile.open(fileName);   //将文件流对象与文件连接起来 
	sprintf(nameBuf, "%s.tmp", nameBuf);
	ofile.open(nameBuf);     //作为输出文件打开
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	char newName[1024];
	char oldName[1024];
	string s;
	std::vector<std::string> ss;
	bool headInit = false;
	char *fileFormat = "";
	bool renameDone = false;
	while (getline(infile, s))
	{
		//cout << s << endl;
		if (!renameDone && s.length() > 0 &&  s.at(0) == ';') {
			if (startsWith(s, ";totalLayer:")) {
				ss = split(s.data() + 1, ":");
				if (ss.size() == 2) {
					int totalLayer = static_cast<int>(strtol(ss[1].c_str(), NULL, 0));
					char *format = "";
					if (totalLayer < 10000) {
						format = "%s/%04d.png";
						fileFormat = "%04d.png";
					}
					else if (totalLayer >= 10000 && totalLayer < 100000) {
						format = "%s/%05d.png";
						fileFormat = "%05d.png";
					}
					else if (totalLayer >= 100000 && totalLayer < 1000000){
						format = "%s/%06d.png";
						fileFormat = "%06d.png";
					}
					else if (totalLayer >= 1000000 && totalLayer < 10000000){
						format = "%s/%07d.png";
						fileFormat = "%07d.png";
					}
					else if (totalLayer >= 10000000 && totalLayer < 100000000) {			//impossible
						format = "%s/%08d.png";
						fileFormat = "%08d.png";
					}
					else{
						format = "%s/%020d.png";
						fileFormat = "%020d.png";
					}
					for (int i = 0; i < totalLayer; i++) {
						snprintf(oldName, sizeof(oldName), "%s/%d.png", dirPath, i + 1);
						snprintf(newName, sizeof(newName), format, dirPath, i + 1);
						rename(oldName, newName);
					}
					renameDone = true;
				}
				//break;
			}
		}

		if (s.length() > 0 && s.find(".png\"") != std::string::npos) {
			ss = split(s.data(), "\"");
			if (ss.size() == 3) {
				std::vector<std::string> _ss = split(ss[1].data(), ".");

				if (_ss.size() > 1) {
					int pngIndex = atoi(_ss[0].data());
					snprintf(newName, sizeof(newName), fileFormat, pngIndex);
					ss[1] = string(newName);
				}
				s = ss[0] + string("\"") + ss[1] + string("\"") + ss[2];
			}
			ofile << s << endl;
		}
		else {
			ofile << s << endl;
		}
	}
	infile.close();             //
	ofile.close();
	remove(fileName);
	rename(nameBuf, fileName);

}

int main(int argc, char **argv) {
	int i;
	map<string, string> valueMap;
	char fileName[1024];
	for (int i = 0; i < argc; i++) {
		cout << "arg" << i << ":" << argv[i] << endl;
	}
	if (argc <3) {
		cout << "error:Invalid parameter,format:" << argv[0] << "input output" << endl;	//Error to UI
		exit(0);
	}
	sprintf(fileName, "%s/run.gcode", argv[1]);
	readGcode(fileName, argv[1]);

	cout << "progress:0" << endl;				//progross to UI
	cout << "info:" << endl;				//progross to UI

}
