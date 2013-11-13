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
	
	infile.open(fulName);
	ofstream tempfile;
	tempfile.open(tempNamec);
	tempfile << "#include <fstream>" << endl;
	while(!infile.eof())
	{
    getline(infile, currLine);
    if(currLine.find("class") != std::string::npos){ //check line contains 'class' 
      flag = true;
    }
    if(currLine.compare("{") != 0 && currLine.compare("}") != 0)
      {
      tempfile << currLine << "\n";
      }
    if(currLine.compare("{") == 0 && !flag)
    {
    currLine = currLine + "std::ofstream logfile;\nlogfile.open(\"log.txt\");\nlogfile << __func__ << std::endl;\nlogfile.close();\n";
    tempfile << currLine << "\n";
    }
    if(currLine.compare("}") == 0)
    {
          currLine = "logfile.open(\"log.txt\");\nlogfile << \"return\" << std::endl;\nlogfile.close();\n" + currLine;
          tempfile << currLine << "\n";
    }
    if(flag){
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
    while (entry = readdir(dp)){ // for every entry in the directory
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

