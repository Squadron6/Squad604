/*
Static Reader

Static Reader uses the AST created by Clang and parses this.
It then forms function pairs denoting which function calls which.
This is then passed to GraphMaker as an unordered map.
*/
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
#include "StaticReader.h"

int totalCalls;

using namespace std;

//unordered_map<string, int> static_parse(string dirName)
//{
//	unordered_map<string, int> funcMap;
//	return funcMap;
//}
/*
  generate_ast
  This function generates a ast dump file for the given cpp file
  at the same time it also analyzes the ast dump file and outputs a
  unordered_map containing the function calls and the number of occurrences
  of each function in the given cpp file
*/
unordered_map<string, int> generate_ast(string fullName, string fileName, unordered_map<string,int> functionMap)
{
	unordered_map<string, int> currentMap = functionMap;
	string command = "clang -cc1 -ast-dump " + fullName + " > " + fileName + ".ast";
	string astFile = fileName+".ast";
	system(command.c_str());
	ifstream infile;

	//opens the ast dump file for reading
	infile.open(astFile.c_str());
	ofstream astLog;

	//the astLog.txt contains the list of method call pairs used to create edges
	astLog.open("astLog.txt", ios::app);
	string currline;
	int lineCount = 0;
	string currFuncName;
	int funcLine = -1;

	//iterates through every line of the dump file determining by key tags whether the line is a function declaration
	while(!infile.eof())
	{
		getline(infile, currline);
		if((currline.find("CompoundStmt") != string::npos) && (currline.find(fileName) != string::npos) && currline.at(0) != ' ' && currline.at(0) != ')')
		{
      istringstream buffer(currline);
      istream_iterator<string> beg(buffer), end;
      vector<string> tokens(beg, end);
      cout << currline << endl;		
      //if a function is found, it is trimmed to have only the function name remaining
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
		//this determines if a line is a function call inside a method
		//the lvalue Function determines a function call in the ast dump file
		if(currline.find("lvalue Function") != string::npos && funcLine > 0)
		{
      string found = currline.substr(currline.find("lvalue Function"));
      istringstream buffer(found);
      istream_iterator<string> beg(buffer), end;
      vector<string> tokens(beg, end);
      //if the line is determined to be a function call, the extra is trimmed off
      for(auto& s: tokens)
      {
        if(s.find("'") != string::npos)
        {
          s.erase(0,1);
          s.erase(s.length()-1, 1);
          //function is logged as a pair between the current method and the function
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
		//this is another type of function call, the bound member functions calls
		if(currline.find("bound member function type") != string::npos && funcLine > 0)
		{
      //the extra inside each line is trimmed
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
      //the function is logged in a pair with current method and the function name
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
	//finishing logging by closing the file and returning the unordered_map of functions and occurences
	astLog.close();
	return currentMap;
}
/*
  explore_directory
  In this adaptation of the exploreInjectDirectory() function, the directories are explored
  to find all the cpp files and generate_ast() is called for each resulting cpp
  file to create an unordered_map of function calls and occurences and a matching 
  pair log file of the static edges
*/
unordered_map<string, int> exploreDirectory(string directory){
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
        //each time a cpp file is found, generate_ast() is called and the new methods are 
        //added to the function map
        funcMap = generate_ast(fullName, fileName, funcMap);
        }
      else if (entry->d_type == isFolder && str!="." && str!=".."){ // if it's a folder, not current or parent
        exploreDirectory(str);
      }
    }
  }
	return funcMap;
}


//int main(void)
//{

//	exploreDirectory("../codeBase/fish-shell-master");
//unordered_map<string, int> funcMap;

//	string test = "echo test this";
//	system(test.c_str());
//	funcMap = generate_ast("../codeBase/fish-shell-master/reader.cpp", "reader.cpp", funcMap);
//	funcMap = generate_ast("../codeBase/fish-shell-master/proc.cpp" , "proc.cpp", funcMap);

//	for(auto& entry: funcMap)
//		cout << entry.first << " : " << entry.second << endl;
//}
