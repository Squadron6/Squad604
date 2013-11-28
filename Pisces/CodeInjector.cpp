/*
Code Injector

The Code Injector goes through all the directories in a codebase
and injects a log statement into each .cpp file such that when
the codebase is compiled and run with user-provided input, a log
file is created documenting which methods are being called.
*/
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cstring>
#include "CodeInjector.h"

using namespace std;

/* 
  inject_code
  Takes in a file name and directory name both as strings.
  It uses these to read in the .cpp file and then does
  a check for each line to see if a log statement
  should be injected.
*/
void inject_code(string fileName, string dirName)
{
  // stream to read in file
  ifstream infile;
  // variable to keep track of current line
  string currLine;

  // store file and directory names as chars
  // and set up renaming convention for files
  // with injected code
  const char * fname = fileName.c_str();
  const char * dname = dirName.c_str();
  string fullName = dirName + "/" + fileName;
  const char * fulName = fullName.c_str();
  string tempName = dirName+"/"+"temp.cpp";
  const char * tempNamec = tempName.c_str();
  string oldName = dirName+"/"+fileName+".old";
  const char * oldNamec = oldName.c_str();

  // there are specific cases for which we don't
  // want to inject, use a flag to keep track
  bool flag = false; 
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
        
  // read in file & set up temporary file that
  // will be the new file with injections
  infile.open(fulName);
  ofstream tempfile;
  tempfile.open(tempNamec);
  tempfile << "#include <fstream>" << endl;

  // go through all the lines in the file
  while(!infile.eof())
  {
    getline(infile, currLine);

    // Check for flags matching in the current line, these are the flags that we should
    // not inject the test code to
    if((!currLine.compare(0, flagcase1.size(), flagcase1) || !currLine.compare(0, flagcase2.size(), flagcase2) ||
    !currLine.compare(0, flagcase3.size(), flagcase3) || !currLine.compare(0, flagcase4.size(), flagcase4) ||
    !currLine.compare(0, flagcase5.size(), flagcase5) || !currLine.compare(0, flagcase6.size(), flagcase6) ||
    !currLine.compare(0, flagcase7.size(), flagcase7) || !currLine.compare(0, flagcase8.size(), flagcase8) ||
    !currLine.compare(0, flagcase9.size(), flagcase9) || !currLine.compare(0, flagcase10.size(), flagcase10) ||
    !currLine.compare(0, flagcase11.size(), flagcase11))
    && currLine.compare(currLine.size()-1, 1, ";"))
    {
      flag = true;
    }
    // if the line matches the flags for which we have to inject code
    // the logging code is injected into the line
    // otherwise the line remains the same
    if(currLine.compare("{") != 0 && currLine.compare("}") != 0)
    {
      tempfile << currLine << "\n";
    }
    // our main check for when to inject is based on curly braces
    // but we only inject if the flag isn't raised
    else if(currLine.compare("{") == 0 && !flag)
    {
      currLine = currLine + "std::ofstream logfile;\nlogfile.open(\"log.txt\", std::ios::app);\nlogfile << __func__ << std::endl;\nlogfile.close();\n";
      tempfile << currLine << "\n";
    }
    else if(currLine.compare("{") == 0 && flag)
    {
      tempfile << currLine << "\n";
    }
    // the next place we want to inject is when the function finishes
    // but only if the flag is not raised
    else if(currLine.compare(0, 1, "}") == 0 && !flag)
    {
      currLine = "logfile.open(\"log.txt\", std::ios::app);\nlogfile << \"return\" << std::endl;\nlogfile.close();\n" + currLine;
      tempfile << currLine << "\n";
    }
    else if(currLine.compare(0, 1, "}") == 0 && flag)
    {
      tempfile << currLine << "\n";
      flag = false;
    }
  }
  // close the file streams and rename the files
  tempfile.close();
  infile.close();
  rename(fulName, oldNamec);
  rename(tempNamec, fulName);
}

/*
 exploreInjectDirectory
 Takes in the path to the codebase directory as a string
 and explores this directory, looking for every .cpp file in order to
 inject log statements into it.
 When there are sub-directories, exploreInjectDirectory explores these as well
 using recursion.
*/
void exploreInjectDirectory(string directory){
  // hex flags used by directory entry types to differentiate
  // between files and folders
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;

  DIR *dp;
  struct dirent *entry;
  const char *dirname = directory.c_str(); //path of directory to read
  dp = opendir(dirname);

  if (dp){
    while (entry = readdir(dp)){ // for every entry in the directory
      // convert directory entry name to a string
      string entrystr(entry->d_name);

      // if it is a cpp file
      if (entry->d_type == isFile && entrystr.compare(entrystr.size()-3, 3, "cpp") == 0){ 
        string fileName = entry->d_name;
        string dirNames = dirname;
        string fullName = dirNames+"/"+ fileName;

        //if the cpp file is found, the code injection method is called on the given file
        inject_code(fileName, dirNames);
      }
      // if it's a folder, not current "." or parent ".."
      else if (entry->d_type == isFolder && entrystr!="." && entrystr!=".."){ 
        exploreInjectDirectory(entrystr);
      }
    }
  }
}
