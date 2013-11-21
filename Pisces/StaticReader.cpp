//Analyser cpp file
#include <string>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <vector>
#include <iterator>
#include <sstream>
#include "DynamicReader.h"

int totalCalls;
//input file

//uses output of CodeReader with Colourizer to create a new output
//consisting of data to be use by GraphMakeri
using namespace std;

unordered_map<string, int> static_parse(string dirName)
{
	unordered_map<string, int> funcMap;
	return funcMap;
}

unordered_map<string, int> generate_ast(string fullName, string fileName, unordered_map<string,int> functionMap)
{
	unordered_map<string, int> currentMap = functionMap;
	string command = "clang -cc1 -ast-dump " + fullName + " > " + fileName + ".ast";
	string astFile = fileName+".ast";
	system(command.c_str());
	ifstream infile;
	infile.open(astFile.c_str());
	ofstream astLog;
	astLog.open("astLog.txt", ios::app);
	string currline;
	int lineCount = 0;
	string currFuncName;
	int funcLine = -1;
	while(!infile.eof())
	{
		getline(infile, currline);
		if((currline.find("CompoundStmt") != string::npos) && (currline.find(fileName) != string::npos) && currline.at(0) != ' ' && currline.at(0) != ')')
		{
		istringstream buffer(currline);
		istream_iterator<string> beg(buffer), end;
		vector<string> tokens(beg, end);
		cout << currline << endl;		
		for(auto& s: tokens)
		{
			if(s.find("(") != string::npos)
			{
			s.erase(s.find("("), string::npos);
			if(!isalpha(s[0]))
			{
			s.erase(0,1);
			}
			currFuncName = s;
			funcLine = lineCount;
			break;
			}
		}
		//cout << currline << endl << lineCount << endl;
		}
		if(currline.find("lvalue Function") != string::npos && funcLine > 0)
		{
		string found = currline.substr(currline.find("lvalue Function"));
		istringstream buffer(found);
		istream_iterator<string> beg(buffer), end;
		vector<string> tokens(beg, end);

		for(auto& s: tokens)
		{
			if(s.find("'") != string::npos)
			{
			s.erase(0,1);
			s.erase(s.length()-1, 1);
			astLog << currFuncName << " " << s << endl;
			unordered_map<string, int>::const_iterator got = currentMap.find(s);
			if(got == currentMap.end())
			{
				currentMap.insert(make_pair(s, 1));
			}
			else
			{
				int newCount = got->second + 1;
				currentMap[s] = newCount;
			}
			break;
			}
		}
		}
		if(currline.find("bound member function type") != string::npos && funcLine > 0)
		{
		string found = currline.substr(currline.find(">'") + 2);
		istringstream buffer(found);
		istream_iterator<string> beg(buffer), end;
		vector<string> tokens(beg,end);
		
		string functionName = tokens[0];
		if(functionName.find(">") != string::npos)
		{
		functionName.erase(0,2);
		}
		if(functionName.find(".") != string::npos)
		{
		functionName.erase(0,1);
		}
		astLog << currFuncName << " " << functionName << endl;
                unordered_map<string, int>::const_iterator got = currentMap.find(functionName);
                if(got == currentMap.end())
                        {
                                currentMap.insert(make_pair(functionName, 1));
                        }
                        else
                        {
                                int newCount = got->second + 1;
                                currentMap[functionName] = newCount;
                        }
		
		}
		lineCount++;
		
	}
	astLog.close();
	return currentMap;
}

void exploreDirectory(string directory){
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = directory.c_str(); //path of directory to read
  unordered_map<string, int> funcMap;
        dp = opendir(dirname);

  if (dp){
    while (entry = readdir(dp)){ // for every entry in the directory
      string str(entry->d_name);
      if (entry->d_type == isFile && str.compare(str.size()-3, 3, "cpp") == 0){ // if it is a cpp file
        string fileName = entry->d_name;
        string dirNames = dirname;
        string fullName = dirNames+"/"+ fileName;
        cout << fullName << " " << fileName << endl;
        funcMap = generate_ast(fullName, fileName, funcMap);
        }
      else if (entry->d_type == isFolder && str!="." && str!=".."){ // if it's a folder, not current or parent
        exploreDirectory(str);
      }
    }
  }
}


int main(void)
{

	exploreDirectory("../codeBase/fish-shell-master");
unordered_map<string, int> funcMap;

	string test = "echo test this";
	system(test.c_str());
	funcMap = generate_ast("../codeBase/fish-shell-master/reader.cpp", "reader.cpp", funcMap);
//	funcMap = generate_ast("../codeBase/fish-shell-master/proc.cpp" , "proc.cpp", funcMap);

//	for(auto& entry: funcMap)
//		cout << entry.first << " : " << entry.second << endl;
}
