//GraphMaker header files
#include <unordered_map>

#ifndef GraphMaker_H
#define GraphMaker_H
using namespace std;


// Initialize the linked list with a dummy start node
void initial_linked_list();

// Setup the python file that will be used as input to Ubigraph
void py_setup(std::ofstream& dumFile);

// Sets the iterator back to the start node
void reset_node_ptr();

// Actuall writes to the python the nodes contained in the linked list
void create_nodes_python(std::ofstream& dumFile);

// Converts a hex color string to a string that python understands
string get_hex_color(string color);

// Create in memory a linked list of nodes that represent function (each node is unique)
void create_linked_list_nodes(unordered_map<string, string> map);

// Create edges between nodes (actually writes the edges to the python file)
void create_edges(std::ifstream& Infile, std::ofstream& dumFile);

// Recolors nodes after dynamic function call frequency has been calculates
void recolor_nodes(unordered_map<string, string> map, std::ofstream& dumFile);

#endif