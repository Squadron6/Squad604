/*
Code reader

For every .cpp file that exists in codebase
read each line of code and store function calls for colour and link function
*/
#include <iostream>
#include <stdio.h>
#include <dirent.h>

using namespace std;

int main(void){
  DIR *dp;
  struct dirent *entry;
  const char *dirname = "."; //path of directory to read
  dp = opendir(dirname);
  if (dp){
    while (entry = readdir(dp)){
      cout << entry->d_name << endl;
    }
  }
}
