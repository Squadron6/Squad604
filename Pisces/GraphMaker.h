//GraphMaker header files 
#include <unordered_map>

#ifndef GraphMaker_H
#define GraphMaker_H
using namespace std;

void initial_linked_list();

void py_setup(std::ofstream& dumFile);

void reset_node_ptr();

void create_nodes(std::ofstream& dumFile);

string get_hex_color(string color);

void create_nodes(unordered_map<string, string> map);

void create_edges(std::ifstream& Infile, std::ofstream& dumFile);

void recolor_nodes(unordered_map<string, string> map, std::ofstream& dumFile);

#endif