#include <unordered_map>
#include <iostream>
#include <string>
#include "Colourizer.h"

//Colour Function
//
//store number of occurences in HashMap

uint24_t RGB_value;

typedef std:unordered_map<std::string, std::uint24_t RGB_value> colour_map;
// Logic -

using namespace std;

int RGB_converter = 510;

int max, tempMax;
float buffer;
unordered_map<string, int> output;
    
int find_max(unordered_map input)
{
    max = 0;
    //min = std::numeric_limits<int>::max();
    
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (max < it->value) {
            max = it->value;
        }
    }
    return max;
}

unordered_map convert_to_RGB(unordered_map input) {
    
    for (auto it = input.begin(); it != input.end(); ++it) {
        // Calculate RGB value here.
        buffer = (float) (it->second/max);
        
        if (510.0*buffer > 255.0) {
            // Red goes to 255, green goes down.
            int temp = 16711680 + (255 - (510*(int) buffer - 255)) * 256;
            RGB_value = hex << temp;
            
            //0x00ff00 (Green)
        }
        else {
            // Green is ff, red increments
            int temp = (510*(int) buffer) * 65535 + 65280;
            RGB_value = hex << temp;
        }
        colour_map[it->first] = RGB_value;
    }
}

//int main() {
//    
//}

//0xFFFFFFFF
//Exaple = #xxxxxx
//'#ff0000'