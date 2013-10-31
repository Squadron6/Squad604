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
using namespace std;

bool isMethod(string currLine, string nextLine)
{
        if((currLine.find("static") != string::npos || currLine.find("void") != string::npos
        || currLine.find("const") != string::npos || currLine.find("signed") != string::npos
        || currLine.find("int") != string::npos || currLine.find("long") != string::npos))
        {
                if(nextLine.compare("{") == 0)
                {
                return true;
                }
        }
        else
        return false;
}

void inject_code(string fileName)
{
	ifstream infile;
	string currLine;
	int lineCOunt = 1;
	const char * fname = fileName.c_str();
	infile.open(fname);
	ofstream tempfile;
	tempfile.open("temp.cpp");
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
    currLine = currLine + "ofstream logfile;\nlogfile.open(\"log.txt\");\nlogfile << __func__ << endl;\n";
    tempfile << currLine << "\n";
    }
    if(currLine.compare("}") == 0)
    {
          currLine = "ofstream logfile;\nlogfile.open(\"log.txt\");\nlogfile << \"return\" << endl;\n" + currLine;
          tempfile << currLine << "\n";
    }
 	
	}
	tempfile.close();
}

int main(void){
  /**** NOTE START
        integrate the following into the pre-existing code injection stuff 
        based on the directory path, the following code will go through all the .cpp files 
        TODO: traverse subdirectories & look for cpp files recursively; pull this out into a separate method? ****/
  unsigned char isFile = 0x8;
  DIR *dp;
  struct dirent *entry;
  const char *dirname = "."; //path of directory to read
  dp = opendir(dirname);
  
  /* the next 2 lines are to-be-removed */
  ifstream testfile;
  string thing;

  if (dp){
    while (entry = readdir(dp)){ // for every entry in the directory
      string str(entry->d_name);
      if (entry->d_type == isFile && str.compare(str.size()-3, 3, "cpp") == 0){ // if it is a cpp file
  /****** NOTE END ******/
        cout << entry->d_name << endl; // print out the name of the file
        testfile.open(entry->d_name);
        getline(testfile, thing); //just going to dump out the first line for testing
        cout << thing << endl;
        testfile.close();
      }
    }
  string currLine;
  string nextLine;
  ifstream infile;
  int lineCount = 1;
  infile.open("proc.cpp");
  getline(infile, currLine);
  while (!infile.eof())
	{
	getline(infile, nextLine);
	if(isMethod(currLine, nextLine))
		{
			int endCount = lineCount+2;
			
			string endLine;
			getline(infile,endLine);
			cout << currLine << endl << "begining of method: " << lineCount << endl;
			while(endLine.compare("}") != 0)
			{
			getline(infile,endLine);
			endCount++;
			}
			cout << endLine << endl << "end of method: " << endCount << endl;
			getline(infile, nextLine);
			lineCount = endCount;
		}
	currLine = nextLine;	
	lineCount++;
	}
  inject_code("proc.cpp");
	
 }
}

