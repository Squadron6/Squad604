#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include "Colourizer.h"
#include <iomanip>

// Member variables
using namespace std;
int RGB_converter = 510;

// Obtain the max number of occurrences
static int find_max(std::unordered_map<string, int> input)
{
    int max = 0;
    
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (max < it->second) {
            max = it->second;
        }
    }
    return max;
}

// Obtain the min number of occurences
static int find_min(std::unordered_map<string, int> input)
{
    int min = std::numeric_limits<int>::max();
    
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (min > it->second) {
            min = it->second;
        }
    }
    return min;
}

// Given a Hashmap which stores a set of a function and its number of occurrences, calculate appropirate
// RGB values and store them in a new Hashmap.
static std::unordered_map<string, string> convert_to_RGB(std::unordered_map<string, int> input, int max, int min) {
    
    std::unordered_map<string, string> colour_map;
    std::stringstream RGB_value;
    int temp;
    float buffer;
    std::string output_buffer;
    max = max - min;
    
    for (auto it = input.begin(); it != input.end(); ++it) {
        
        // Calculate RGB value here.
        std::stringstream RGB_value;
        buffer = ((float) (it->second - min) / (float) max);
    
        if ((510.0 * buffer) > 255.0) {
            
            // The colour changes from Yellow to Red
            temp = (int) std::round(16776960 - ((int)(510 * buffer - 255)) * 256);
            RGB_value << "0x" << setw(6) << std::setfill('0') << std::hex << temp;
        }
        else {
            
            // The colour changes from Green to Yellow
            temp = (int) std::round(((int) (510 * buffer)) * 65536 + 65280);
            RGB_value << "0x" << setw(6) << std::setfill('0') << std::hex << temp;
        }

        colour_map[it->first] = RGB_value.str();
    }
    return colour_map;
}

int main() {
//    TEST CASES
//
//    unordered_map <string, int> sample;
//    sample["test1"] = 1;
//    sample["test2"] = 2;
//    sample["test3"] = 3;
//    sample["test4"] = 4;
//    sample["test5"] = 5;
//    sample["test6"] = 6;
//    sample["test7"] = 7;
//    sample["test8"] = 8;
//    sample["test9"] = 9;
//    sample["test10"] = 10;
//
//    cout << find_max(sample) << endl;
//    unordered_map<string, string> sample_RGB = convert_to_RGB(sample, find_max(sample), find_min(sample));
//
//    //cout << sample_RGB["test4"] << endl;
}
