#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include <cstring>
#include <stack>
#include <unordered_map>
#include "CodeInjector.h"
#include "DynamicReader.h"
using namespace std;

int main(int argc, char* argv[]){
  string dirname = "../../fish-shell-master"; //path to codebase
  //string logname = "../../fish-shell-master/log.txt"; 
  string logname = "log.txt"; 
  string fishnav = "cd ../../fish-shell-master";
  string fishauto = "(cd ../../fish-shell-master && autoconf)";
  string fishconf = "(cd ../../fish-shell-master && ./configure --prefix=$HOME)";
  string fishmake = "make install -C ../../fish-shell-master";
  string echotest = "pwd";
  string fishrun = "fish";
  string cmd = "ls";
  string cmd2 = "exit";
  //string fishmake = "(cd ../../fish-shell-master && make install)";

  system(fishnav.c_str());
  system(fishauto.c_str());
  system(fishconf.c_str());
  system(fishmake.c_str());

  exploreDirectory(dirname);
  system(echotest.c_str());
  system(cmd2.c_str());
  system(fishrun.c_str());
  system(cmd.c_str());
  system(cmd2.c_str());

	unordered_map<string, int> functionMap;
	functionMap = parse_log(logname);
	for(auto& x: functionMap)
		cout<< x.first << ": " << x.second << endl;
    
}
