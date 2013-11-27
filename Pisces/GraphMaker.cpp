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

using namespace std;

struct node  {
    string node_name;
    string color;
    int id;
    node* next_edge;
    node* next;
};

//id count
int id_count = 0;

//stream to read from readfile
//ifstream infile;

//stream to write to writefile
//ofstream outfile;

//default graph name
string Graph = "G";

//default server
string server = "http://127.0.0.1:20738/RPC2";

//start of linked list
node* start;

//iterator for linked list of nodes
node* node_ptr;

//the input log file name
//const char* readfile = "astLog.txt";

//const char* readfile_dynamic = "log.txt";

//the output python file name
//const char* writefile = "graph.py";

//creates the first node of the linked list
void initial_linked_list(){
    
    //initialize linked list
    start = new node;
    start->node_name = "start";
    start->color = " ";
    start->id = -1;
    start->next = NULL;
    node_ptr = start;
    cout << "**Creating linked list, the start node: " << node_ptr->node_name <<endl;
}

/*setup the python file with nessesary libraries, server and graph. Clears
 everything from Ubigraph's canvas */
void py_setup(std::ofstream& dumFile){
    dumFile << "import xmlrpclib" << endl;
    dumFile << "import time" << endl << endl;   //used for a slower growth ie. time.sleep(6)
    dumFile << "server = xmlrpclib.Server('" << server << "')" << endl;
    dumFile << Graph << " = server.ubigraph" << endl;
    dumFile << Graph << ".clear()" << endl << endl;
    dumFile << Graph << ".set_edge_style_attribute(0," << "\"strength\"" << ", str(0.1))" << endl;
}

/* set node_ptr back to start */
void reset_node_ptr(){
    node_ptr = start;
}

/* iterates through the nodes, starting from wherever the node_ptr is add to python file*/
void create_nodes(std::ofstream& dumFile){
    if(node_ptr != NULL){
        cout << "node ptr not null..start traversing" << endl;
        while ( node_ptr-> next != NULL ) {
            cout << "node name: " << node_ptr->node_name << endl;
            
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
        cout << "node name: " << node_ptr->node_name << endl << "sanity check complete" << endl;
    }
    reset_node_ptr();
}

string get_hex_color(string color){
    string hex_color = color.substr(2,7);
    string hash_tag = "#";
    hash_tag.append(hex_color);
    cout << "The new hex color is: " << hash_tag << endl;
    return hash_tag;
}

/* KEEP creates linked list of nodes, sets node_ptr back to start */
void create_nodes(unordered_map<string, string> map){
    for ( auto it = map.begin(); it != map.end(); ++it ){
        string name =  it->first;
        string Color =  it->second;
        //std::cout << " " << it->first << ":" << it->second;
        
        //create a new node, add it to linked list of nodes
        node* start2;
        start2 = new node;
        start2->node_name = name;
        start2->color = get_hex_color(Color);
        start2->id = id_count;
        start2->next = NULL;
        node_ptr->next = start2;
        node_ptr = node_ptr->next;
        id_count++;
        cout << "**Creating linked list, the next node:" << node_ptr->node_name << " color:" << node_ptr->color <<  " id: " << node_ptr->id <<  endl;
        
    }
    reset_node_ptr();
    cout << "setting node ptr back to start: " << node_ptr->node_name <<endl;
}

void create_edges(std::ifstream& Infile, std::ofstream& dumFile){
    cout << "**reading the astLog.txt here is what i found: " << endl;
    string current_line;
    while (getline(Infile, current_line)) {
        //get the node names
        string from;
        string to;
        std::istringstream string_stream(current_line);
        string_stream >> from;
        string_stream >> to;
        cout << "**from: " << from << " to: " << to << endl;
        
        int id_from;
        int id_to;
        string name_from;
        string name_to;
        
        
        if(node_ptr != NULL){
            while ( node_ptr-> next != NULL ) {
                if( node_ptr->node_name == from ){
                    //cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and from is: " << from << endl;
                    id_from = node_ptr->id;
                    name_from = node_ptr->node_name;
                    
                }
                if( node_ptr->node_name == to ){
                    //cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and to is: " << from << endl;
                    id_to = node_ptr->id;
                    name_to = node_ptr->node_name;
                    
                }
                node_ptr = node_ptr->next;
            }
            //make sure to take care of the last node here
            if( node_ptr->node_name == from ){
                //cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and from is: " << from << endl;
                id_from = node_ptr->id;
                name_from = node_ptr->node_name;
            }
            if( node_ptr->node_name == to ){
                //cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and to is: " << from << endl;
                id_to = node_ptr->id;
                name_to = node_ptr->node_name;
            }
        }
        reset_node_ptr();
        cout << "@@@the nodes that were found - from: "<< name_from << " id:" << id_from <<" to: "<< name_to << " id:" << id_to << endl;
        if( id_from != id_to){
            //DOESNT TAKE INTO ACCOUNT RECURSION
            //IF CALLS ARE MADE MORE THAN ONCE BETWEEN METHODS THEY GET CLOSER TOGETHER, DO WE WANT THAT???
            dumFile << "G.new_edge(" << (id_from) << "," << id_to << ")" << endl << endl;
        }
    }
    
}


//After the initial static graph is made, this takes the function call frequency from dynamic stuff
void recolor_nodes(unordered_map<string, string> map, std::ofstream& dumFile){
    for ( auto it = map.begin(); it != map.end(); ++it ){
        string func_name = it->first;
        string color = it->second;
        
        if(node_ptr != NULL){
            while ( node_ptr-> next != NULL ) {
                if( func_name == node_ptr->node_name ){
                    string color_hex = get_hex_color(color);
                    dumFile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << color_hex << "')" << endl;
                }
                node_ptr = node_ptr->next;
            }
        }
        //take care of the last node in the linked list
        if( func_name == node_ptr->node_name ){
            string color_hex = get_hex_color(color);
            dumFile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << color_hex << "')" << endl;
        }
        //make node_ptr point to the start of the linked list again
        reset_node_ptr();
    }
}

//
//int main () {
//    
//    cout<< "**static reader creating unordered map" << endl;
//    unordered_map<string, int> funcMap;
//    funcMap = exploreDirectory("../codeBase/fish-shell-master/");
//    cout << "**done creating the map, now printing pairs.." <<endl;
//    
//    std::cout << "**funcMap contains:";
//    for ( auto it = funcMap.begin(); it != funcMap.end(); ++it )
//        std::cout << " " << it->first << ":" << it->second;
//    std::cout << endl << endl;
//    
//    //Create the Colorizer hash map (again passing in dummy map, since funcMap doesnt seem to work)
//    unordered_map<string, string> colored_map = convert_to_RGB(funcMap, find_max(funcMap), find_min(funcMap));
//    
//    
//    //this will be the static graph
//    create_nodes(colored_map);
//    
//    //incase we don't start at the beginning of the node list, reset the node pointer
//    reset_node_ptr();
//    
//    
//    //start coloring the nodes according to the dynamic calls to methods
//    unordered_map<string, int> funcMap2;
//    funcMap2 = parse_log(readfile_dynamic);
//    unordered_map<string, string> colored_map_dynamic = convert_to_RGB(funcMap2, find_max(funcMap2), find_min(funcMap2));
//    
//    //start creating python script for ubigraph
//    infile.open(readfile);      //use the log file to generate edges
//    outfile.open(writefile);    //this is the output python file
//    
//    cout << "creating python script" << endl;
//    py_setup(outfile);
//    //sanity check: have nodes been created?
//    
//    cout << "sanity check: can we traverse nodes?" << endl;
//    create_nodes(outfile);
//    
//    //create edges between nodes
//    create_edges(infile,outfile);
//    
//    //this will be the dynamic runtime color added to the static graph
//    recolor_nodes(colored_map_dynamic);
//    
//    outfile.close();
//    infile.close();
//    cout << "finished creating python script" << endl;
//    
//    
//    //parse_file();
//    //traverse_nodes();
//    
//    
//    return 0;
//    
//}

/*
 
 g++ -c -std=c++11 Colourizer.cpp
 g++ -c -std=c++11 StaticReader.cpp
 g++ -c -std=c++11 main.cpp
 g++ -o final main.o StaticReader.o Colourizer.o
 ./final
 
 */