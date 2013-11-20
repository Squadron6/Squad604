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

void exploreDirectory(string directory){
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = directory.c_str(); //path of directory to read
  dp = opendir(dirname);

  if (dp){
    while (entry = readdir(dp)){ // for every entry in the directory
      string str(entry->d_name);
      if (entry->d_type == isFile && str.compare(str.size()-3, 3, "cpp") == 0){ // if it is a cpp file
        string fileName = entry->d_name;
        string dirNames = dirname;
        string fullName = dirNames+"/"+ fileName;
      }
      else if (entry->d_type == isFolder && str!="." && str!=".."){ // if it's a folder, not current or parent
        exploreDirectory(str);
      }
    }
  }
}

void generate_ast(string fullName, string fileName)
{
	string command = "clang++ -fsyntax-only -Xclang -ast-dump " + fullName + " > " + fileName + ".ast";
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
			cout << s << endl;
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
			break;
			}
		}
		}
		lineCount++;
		
	}
	astLog.close();
}

int main(void)
{
	string test = "echo test this";
	system(test.c_str());
	generate_ast("../codeBase/fish-shell-master/proc.cpp" , "proc.cpp");
}
