//Colourizer header file

#include <unordered_map>

#ifndef Colourizer_h
#define Colourizer_h

// Obtain the max number of occurrences
int find_max(std::unordered_map<std::string, int> input);

// Obtain the min number of occurrences
int find_min(std::unordered_map<std::string, int> input);

// Given a Hashmap which stores a set of a function and its number of occurrences, calculate appropirate
// RGB values and store them in a new Hashmap.
std::unordered_map<std::string, std::string> convert_to_RGB(std::unordered_map<std::string, int> input, int max, int min);

#endif