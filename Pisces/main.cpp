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

//struct node  {
//    string node_name;
//    string color;
//    int id;
//    node* next_edge;
//    node* next;
//};

//id count
//int id_count = 0;

//stream to read from readfile
ifstream infile;

//stream to write to writefile
ofstream outfile;

//default graph name
string Graph = "G";

//default server
//string server = "http://127.0.0.1:20738/RPC2";

//start of linked list
//node* start;

//iterator for linked list of nodes
//node* node_ptr;

//the input log file name
//const char* readfile = "astLog.txt";

//const char* readfile_dynamic = "log.txt";

//the output python file name
//const char* writefile = "graph.py";


int main () {
    
    const char* readfile_dynamic = "log.txt";
    const char* readfile = "astLog.txt";
    const char* writefile = "graph.py";
    
    initial_linked_list();
    
    cout<< "**static reader creating unordered map" << endl;
    unordered_map<string, int> funcMap;
    funcMap = exploreDirectory("../codeBase/fish-shell-master/");
    cout << "**done creating the map, now printing pairs.." <<endl;
    
    std::cout << "**funcMap contains:";
    for ( auto it = funcMap.begin(); it != funcMap.end(); ++it )
        std::cout << " " << it->first << ":" << it->second;
    std::cout << endl << endl;
     
    //Create the Colorizer hash map (again passing in dummy map, since funcMap doesnt seem to work)
    unordered_map<string, string> colored_map = convert_to_RGB(funcMap, find_max(funcMap), find_min(funcMap));
    
    //initialize linked list
//    start = new node;
//    start->node_name = "start";
//    start->color = " ";
//    start->id = -1;
//    start->next = NULL;
//    node_ptr = start;
//    cout << "**Creating linked list, the start node: " << node_ptr->node_name <<endl;
    
    //this will be the static graph
    create_nodes(colored_map);
    
    //incase we don't start at the beginning of the node list, reset the node pointer
    reset_node_ptr();
    
    
    //start coloring the nodes according to the dynamic calls to methods
    unordered_map<string, int> funcMap2;
    funcMap2 = parse_log(readfile_dynamic);
    unordered_map<string, string> colored_map_dynamic = convert_to_RGB(funcMap2, find_max(funcMap2), find_min(funcMap2));
    
    //start creating python script for ubigraph
    infile.open(readfile);      //use the log file to generate edges
    outfile.open(writefile);    //this is the output python file
    
    cout << "creating python script" << endl;
    py_setup(outfile);
    //sanity check: have nodes been created?
    
    cout << "sanity check: can we traverse nodes?" << endl;
    create_nodes(outfile);
    
    //create edges between nodes
    create_edges(infile,outfile);
    
    //this will be the dynamic runtime color added to the static graph
    recolor_nodes(colored_map_dynamic, outfile);
    
    outfile.close();
    infile.close();
    cout << "finished creating python script" << endl;
    
    
    //parse_file();
    //traverse_nodes();
    
    
return 0;

}

/*
 
g++ -c -std=c++11 Colourizer.cpp
g++ -c -std=c++11 StaticReader.cpp
g++ -c -std=c++11 main.cpp
g++ -o final main.o StaticReader.o Colourizer.o
./final
 
*/