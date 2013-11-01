/*
Code Injector

For every .cpp file that exists in codebase
read each line of code and store function calls for colour and link function
*/
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

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
	
	infile.open(fulName);
	ofstream tempfile;
	tempfile.open(tempNamec);
	tempfile << "#include <fstream>" << endl;
	while(!infile.eof())
	{
    getline(infile, currLine);
    if(currLine.compare("{") != 0 && currLine.compare("}") != 0)
      {
      tempfile << currLine << "\n";
      }
    if(currLine.compare("{") == 0)
    {
    currLine = currLine + "ofstream logfile;\nlogfile.open(\"log.txt\");\nlogfile << __func__ << endl;\nlogfile.close();\n";
    tempfile << currLine << "\n";
    }
    if(currLine.compare("}") == 0)
    {
          currLine = "logfile.open(\"log.txt\");\nlogfile << \"return\" << endl;\nlogfile.close();\n" + currLine;
          tempfile << currLine << "\n";
    }
 	
	}
	tempfile.close();
	infile.close();
	rename(fulName, oldNamec);
	rename(tempNamec, fulName);
}

void exloreDirectory(string directory){
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = directory.c_str(); //path of directory to read
  dp = opendir(dirname);

  ifstream testfile;
  string thing;

  if (dp){
    while (entry = readdir(dp)){ // for every entry in the directory
      string str(entry->d_name);
      if (entry->d_type == isFile && str.compare(str.size()-3, 3, "cpp") == 0){ // if it is a cpp file
        cout << entry->d_name << endl; // print out the name of the file
        testfile.open(entry->d_name);
        getline(testfile, thing); //just going to dump out the first line for testing
        cout << thing << endl;
        testfile.close();
      }
      else if (entry->d_type == isFolder && str!="." && str!=".."){ // if it's a folder, not current or parent
        //exploreDirectory(str); //how can this be recursive? it says exploreDirectory was not declared in this scope... do we need to create a .h file?
      }
    }
  }
}

int main(void){
  /**** NOTE START
        integrate the following into the pre-existing code injection stuff 
        based on the directory path, the following code will go through all the .cpp files 
        TODO: traverse subdirectories & look for cpp files recursively; pull this out into a separate method? ****/
  unsigned char isFile = 0x8;
  unsigned char isFolder = 0x4;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = "../codeBase/fish-shell-master"; //path of directory to read
  dp = opendir(dirname);
  
  /* the next 2 lines are to-be-removed */
  ifstream testfile;
  string thing;

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
        // we need to explore this folder too, checking for .cpp files
        // this likely means that this funtionality needs to pulled out into a function like...
        // void directoryExplorer(string directoryPath)
        //  use the code from the above note
        //  and for the folder case (this condition!) then recursively call this method with new folder as path
        // not sure if folder path needs ./ to be prepended
        cout << "this is a folder! " << entry->d_name << endl;
      }
    }
  /****** NOTE END ******/
  // I think that the following needs to be moved into exploreDirectory into the file case
 }
}

