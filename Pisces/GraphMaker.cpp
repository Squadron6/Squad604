/*
  Graph Maker

  The Graph Maker takes in the output from the Static and Dynamic
  Readers and from it, creates a Python script that will be run
  in order to create the function call graph with UbiGraph.
*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Colourizer.h"
#include "StaticReader.h"
#include "DynamicReader.h"
#include "GraphMaker.h"

using namespace std;

/**
 @author Sonika Prakash
 */

/* Representation of a single node. Each node contains the node name, color and a unique node id (next points to another node, in no particular order, it's just part of the linked list */
struct node  {
    string node_name;
    string color;
    int id;
};

/* Keeps track of how many unique nodes were created. */
int id_count = 0;

/* A default graph name that will be used in the python file */
string Graph = "G";

/* Default server used in python file */
string server = "http://127.0.0.1:20738/RPC2";

/* Start of linked list */
node* start;

/* Iterator for linked list of nodes */
node* node_ptr;

/** Creates the first node of the linked list. Not shown in graph, this is just a starting point */
void initial_linked_list(){
    
    // initialize the first node with dummy values, and make the iterator point to it
    start = new node;
    start->node_name = "start";
    start->color = " ";
    start->id = -1;
    start->next = NULL;
    node_ptr = start;
}

/** Setup the python file with nessesary libraries, graph attributes, server name and graph name. Clears everything from Ubigraph's current screen
 @param dumFile The output file stream used to write to the python file
 */
void py_setup(std::ofstream& dumFile){
    dumFile << "import xmlrpclib" << endl;
    dumFile << "import time" << endl << endl;
    dumFile << "server = xmlrpclib.Server('" << server << "')" << endl;
    dumFile << Graph << " = server.ubigraph" << endl;
    dumFile << Graph << ".clear()" << endl << endl;
    dumFile << Graph << ".set_edge_style_attribute(0," << "\"strength\"" << ", str(0.1))" << endl;
}

/** Set node_ptr back to start */
void reset_node_ptr(){
    node_ptr = start;
}

/** Iterate through the linked list of nodes and add them to the python file
 @param dumFile The output file stream used to write to the python file
 */
void create_nodes_python(std::ofstream& dumFile){
    if(node_ptr != NULL){
        while ( node_ptr-> next != NULL ) {
            //add node to python file
            if (node_ptr->id > -1) { //make sure not to include start node
                dumFile << "G.new_vertex_w_id(" << node_ptr->id << ")" << endl;
                dumFile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << node_ptr-> color << "')" << endl;
                dumFile << "G.set_vertex_attribute(" << node_ptr->id << ",'shape', 'sphere')" << endl;
                dumFile << "G.set_vertex_attribute(" << node_ptr->id  << ",\"label\", \"" << node_ptr->node_name  <<   "\")" << endl;
            }
            
            //move to the next node
            node_ptr = node_ptr->next;
        }
    }
    reset_node_ptr();
}

/** Converts a hex color string of the format 0xffffff to #ffffff
 @param color, the string to be converted
 @return the new string
 */
string get_hex_color(string color){
    string hex_color = color.substr(2,7);
    string hash_tag = "#";
    hash_tag.append(hex_color);
    cout << "The new hex color is: " << hash_tag << endl;
    return hash_tag;
}

/** Create linked list of nodes based on the unordered map of function:color pairs
 @param map the unordered map with key:function and value:color
 */
void create_linked_list_nodes(unordered_map<string, string> map){
    for ( auto it = map.begin(); it != map.end(); ++it ){
        string name =  it->first;
        string Color =  it->second;
        
        //Create a new node, set it's properties and add it to linked list of nodes
        node* start2;
        start2 = new node;
        start2->node_name = name;
        start2->color = get_hex_color(Color);
        start2->id = id_count;
        start2->next = NULL;
        node_ptr->next = start2;
        node_ptr = node_ptr->next;
        // Increment the unique number of nodes count
        id_count++;
        
    }
    reset_node_ptr();
}

/** Create edges between the nodes that have been created already (note: an edge cannot be created between a node and itself)
 @param Infile, stream that reads a log file containing pairs of functions that call eachother (i.e. the edges)
 @param dumFile, stream that writes to the output python file
 */
void create_edges(std::ifstream& Infile, std::ofstream& dumFile){
    // Read in each line of the log file
    string current_line;
    while (getline(Infile, current_line)) {
        // Get the name of the caller function and callee function (ie. the from and to respectively)
        string from;
        string to;
        std::istringstream string_stream(current_line);
        string_stream >> from;
        string_stream >> to;
        
        // Variables that will keep track of the node ids and names as we iterate through the linked list
        int id_from;
        int id_to;
        string name_from;
        string name_to;
        
        
        if(node_ptr != NULL){
            // Iterate through the linked list and find the corresponding caller and callee nodes so that we know their id's
            while ( node_ptr-> next != NULL ) {
                if( node_ptr->node_name == from ){
                    id_from = node_ptr->id;
                    name_from = node_ptr->node_name;
                    
                }
                if( node_ptr->node_name == to ){
                    id_to = node_ptr->id;
                    name_to = node_ptr->node_name;
                    
                }
                node_ptr = node_ptr->next;
            }
            //make sure to take care of the last node here
            if( node_ptr->node_name == from ){
                id_from = node_ptr->id;
                name_from = node_ptr->node_name;
            }
            if( node_ptr->node_name == to ){
                id_to = node_ptr->id;
                name_to = node_ptr->node_name;
            }
        }
        reset_node_ptr();
        if( id_from != id_to){ // Edges cannot be created between a node and itself
            
            //Add the edges to the graph, by writing to the python file
            dumFile << "G.new_edge(" << (id_from) << "," << id_to << ")" << endl << endl;
        }
    }
}


/** After the initial static graph is made, take dynamic function call frequency and based on that recolor the nodes that already exist in the graph. This does not create any new nodes, as the nodes are the same as in the static graph.
 @param map, an unordered map of functions as keys and colors as values
 @param dumFile, the stream used to write to the output python file
 */
void recolor_nodes(unordered_map<string, string> map, std::ofstream& dumFile){
    for ( auto it = map.begin(); it != map.end(); ++it ){
        // Get the function and it's respective color from the map
        string func_name = it->first;
        string color = it->second;
        
        if(node_ptr != NULL){
            while ( node_ptr-> next != NULL ) {
                // Iterate throught the link list of unique nodes to find the node that matches the function name func_name
                if( func_name == node_ptr->node_name ){
                    string color_hex = get_hex_color(color);
                    // Write the color change to the python output file
                    dumFile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << color_hex << "')" << endl;
                }
                node_ptr = node_ptr->next;
            }
        }
        // Take care of the last node in the linked list
        if( func_name == node_ptr->node_name ){
            string color_hex = get_hex_color(color);
            dumFile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << color_hex << "')" << endl;
        }
        // Make node_ptr point to the start of the linked list again
        reset_node_ptr();
    }
}