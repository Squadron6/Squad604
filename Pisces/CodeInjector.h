//Code Injector
//
//For every .cpp file that exists in codebase
//read each line of code and store function calls for colour and link function

#ifndef CodeInjector_h
#define CodeInjector_h
using namespace std;

void inject_code(std::string fileName, std::string dirName);
void exploreDirectory(std::string directory);

#endif