#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_map>
#include "DynamicReader.h"

using namespace std;

unordered_map<string, int> parse_log(string logfile)
<<<<<<< HEAD
{
    unordered_map<string, int> funcMap;
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
            unordered_map<string, int>::const_iterator got = funcMap.find(currline);
            if(got==funcMap.end())
            {
                funcMap.insert(make_pair(currline,1));
            }
            else
            {
                int newCount = got->second + 1;
                funcMap[currline] = newCount;
            }
            functionStack.push(currline);
        }
        else
        {
            string toFunc = functionStack.top();
            functionStack.pop();
            if(!functionStack.empty())
            {
                string fromFunc = functionStack.top();
                outfile << fromFunc << " " << toFunc << "\n";
            }
        }
    }
    infile.close();
    outfile.close();
    funcMap.erase("");
    return funcMap;
=======
{	
	unordered_map<string, int> funcMap;
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
		unordered_map<string, int>::const_iterator got = funcMap.find(currline);
		if(got==funcMap.end())
			{
			funcMap.insert(make_pair(currline,1));
			}
		else
			{
			int newCount = got->second + 1;
			funcMap[currline] = newCount;
			}
		functionStack.push(currline);
		}
	else
		{
		string toFunc = functionStack.top();
		functionStack.pop();
		if(!functionStack.empty())
		{
		string fromFunc = functionStack.top();
		outfile << fromFunc << " " << toFunc << "\n";
		}
		}
	}
	infile.close();
	outfile.close();
	funcMap.erase("");
	return funcMap;	
>>>>>>> 1550a990e227ecc655741aaaf9165262c84cb77e
}



int main(void)
{
    unordered_map<string, int> functionMap;
    functionMap = parse_log("log.txt");
    for(auto& x: functionMap)
        cout<< x.first << ": " << x.second << endl;
}

<<<<<<< HEAD
*/
=======
>>>>>>> 1550a990e227ecc655741aaaf9165262c84cb77e
