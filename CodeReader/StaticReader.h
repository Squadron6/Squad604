#include <unordered_map>

#ifndef StaticReader_h
#define StaticReader_h
using namespace std;

std::unordered_map<std::string, int> static_parse(std::string dirName);
void exploreDirectory(std::string directory);

std::unordered_map<std::string, int> generate_ast(std::string fullName, std::string fileName, std::unordered_map<std::string,int> functionMap);

#endif
