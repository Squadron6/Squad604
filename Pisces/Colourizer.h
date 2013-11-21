//Colourizer header file

#include <unordered_map>

#ifndef Colourizer_h
#define Colourizer_h

static int find_max(std::unordered_map<std::string, int> input);
static int find_min(std::unordered_map<std::string, int> input);

static std::unordered_map<std::string, std::string> convert_to_RGB(std::unordered_map<std::string, int> input, int max, int min);
#endif