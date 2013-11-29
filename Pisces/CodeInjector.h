//Code Injector
//
//For every .cpp file that exists in codebase
//read each line of code and store function calls for colour and link function

#ifndef CodeInjector_h
#define CodeInjector_h
using namespace std;

/*  
    Takes in a file name and directory name both as strings.
    It uses these to read in the .cpp file and then does
    a check for each line to see if a log statement
    should be injected.
*/
void inject_code(std::string fileName, std::string dirName);

/*
    Takes in the path to the codebase directory as a string
    and explores this directory, looking for every .cpp file in order to
    inject log statements into it.
    When there are sub-directories, exploreInjectDirectory explores these as well
    using recursion.
 */
void exploreInjectDirectory(std::string directory);

#endif
