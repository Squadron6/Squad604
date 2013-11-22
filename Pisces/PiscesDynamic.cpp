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
  string dirname = "../../fish-shell-master"; //path to codebase!!!!! TODO: update this? I'm leaving it like this for testing, otherwise you have to delete and re-copy it over every time due to the injection
  string logname = "log.txt"; 
  string fishauto = "(cd ../../fish-shell-master && autoconf)"; // TODO: use dirname var instead?
  string fishconf = "(cd ../../fish-shell-master && ./configure --prefix=$HOME)";
  string fishmake = "make install -C ../../fish-shell-master";
  string fishrun = "fish";

  exploreDirectory(dirname);
  system(fishauto.c_str());
  system(fishconf.c_str());
  system(fishmake.c_str());

  system(echotest.c_str());
  system(cmd2.c_str());
  system(fishrun.c_str());
  // Fish then opens and you have to give it input at this point

	unordered_map<string, int> functionMap;
	functionMap = parse_log(logname);
	for(auto& x: functionMap)
		cout<< x.first << ": " << x.second << endl;
    
}
