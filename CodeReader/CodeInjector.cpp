/*
Code Injector

The Code Injector goes through all the directories in a codebase
and injects a log statement into each .cpp file such that when
the codebase is compiled and run to catch dynamic input, a log
file is created documenting which methods are being called.
*/

#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;
#include "CodeInjector.h"



// inject_code takes in a file name and directory name
// both as strings.
// It uses these to read in the .cpp file and then does
// a check for each line to see if a log statement
// should be injected.
void inject_code(string fileName, string dirName)
{
	ifstream infile;
	string currLine;
	int lineCOunt = 1;
	const char * fname = fileName.c_str();
	const char * dname = dirName.c_str();
	string fullName = dirName + "/" + fileName;
	const char * fulName = fullName.c_str();
	string tempName = dirName+"/"+"temp.cpp";
	const char * tempNamec = tempName.c_str();
	string oldName = dirName+"/"+fileName+".old";
	const char * oldNamec = oldName.c_str();
  bool flag = false; //use to flag cases where we won't inject
  string flagcase1 = "class";
  string flagcase2 = "struct";
  string flagcase3 = "enum";
  string flagcase4 = "static const struct";
  string flagcase5 = "static const";
  string flagcase6 = "const";
  string flagcase7 = "typedef struct";
  string flagcase8 = "static enum";
  string flagcase9 = "extern \"C\" {";
  string flagcase10 = "static struct";
  string flagcase11 = "typedef enum";
	
	infile.open(fulName);
	ofstream tempfile;
	tempfile.open(tempNamec);
	tempfile << "#include <fstream>" << endl;
	while(!infile.eof())
	{
    getline(infile, currLine);
    if((!currLine.compare(0, flagcase1.size(), flagcase1) || !currLine.compare(0, flagcase2.size(), flagcase2) || 
    !currLine.compare(0, flagcase3.size(), flagcase3) || !currLine.compare(0, flagcase4.size(), flagcase4) ||
    !currLine.compare(0, flagcase5.size(), flagcase5) || !currLine.compare(0, flagcase6.size(), flagcase6) ||
    !currLine.compare(0, flagcase7.size(), flagcase7) || !currLine.compare(0, flagcase8.size(), flagcase8) ||
    !currLine.compare(0, flagcase9.size(), flagcase9) || !currLine.compare(0, flagcase10.size(), flagcase10) ||
    !currLine.compare(0, flagcase11.size(), flagcase11))
    && currLine.compare(currLine.size()-1, 1, ";")) 
    {
      flag = true;
      //printf("%s\n", currLine.c_str());
    }
    if(currLine.compare("{") != 0 && currLine.compare("}") != 0)
    {
    tempfile << currLine << "\n";
    }
    else if(currLine.compare("{") == 0 && !flag)
    {
    currLine = currLine + "std::ofstream logfile;\nlogfile.open(\"log.txt\");\nlogfile << __func__ << std::endl;\nlogfile.close();\n";
    tempfile << currLine << "\n";
    }
    else if(currLine.compare("{") == 0 && flag)
    {
    tempfile << currLine << "\n";
    }
    else if(currLine.compare(0, 1, "}") == 0 && !flag)
    {
          currLine = "logfile.open(\"log.txt\");\nlogfile << \"return\" << std::endl;\nlogfile.close();\n" + currLine;
          tempfile << currLine << "\n";
    }
    else if(currLine.compare(0, 1, "}") == 0 && flag)
    {
    tempfile << currLine << "\n";
    flag = false;
    }
	}
	tempfile.close();
	infile.close();
	rename(fulName, oldNamec);
	rename(tempNamec, fulName);
}

// exploreDirectory takes in the path to the codebase directory as a string
// and explores this directory, looking for every .cpp file in order to
// inject log statements into it.
// When there are sub-directories, exploreDirectory explores these as well
// using recursion.
void exploreDirectory(string directory){
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = directory.c_str(); //path of directory to read
  dp = opendir(dirname);

  if (dp){
    while (entry == readdir(dp)){ // for every entry in the directory
      string str(entry->d_name);
      if (entry->d_type == isFile && str.compare(str.size()-3, 3, "cpp") == 0){ // if it is a cpp file
      	string fileName = entry->d_name;
        string dirNames = dirname;
        string fullName = dirNames+"/"+ fileName;
        inject_code(fileName, dirNames);
      }
      else if (entry->d_type == isFolder && str!="." && str!=".."){ // if it's a folder, not current or parent
        exploreDirectory(str);
      }
    }
  }
}

int main(void){
  string dirname = "../../fish-shell-master"; //path to codebase
  //string dirname = "../codeBase/fish-shell-master"; //path to codebase
  exploreDirectory(dirname);
}

