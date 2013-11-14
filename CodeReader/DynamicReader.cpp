#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stack>
#include <cstring>
using namespace std;

void parse_log(string logfile)
{	
	std::stack<string> functionStack;
	ifstream infile;
	ofstream outfile;
	const char * fname = logfile.c_str();
	string currline;
	infile.open(fname);
	outfile.open("parsedLog.log");
	while(!infile.eof())
	{
	getline(infile, currline);
	if(currline.compare("return") != 0)
		{
		functionStack.push(currline);
		}
	else
		{
		string toFunc = functionStack.top();
		functionStack.pop();
		string fromFunc = functionStack.top();
		outfile << fromFunc << " " << toFunc << "\n";
		}
	}
	infile.close();
	outfile.close();
	
}

int main(void)
{
	string peek;
	std::stack<string> funcStack;
	funcStack.push("test");
	funcStack.push("test2");
	peek = funcStack.top();
	cout<<endl<<"peeked at: "<<peek<<endl;
	parse_log("test.log");
}

