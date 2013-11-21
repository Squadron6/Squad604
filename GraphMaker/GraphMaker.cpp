#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
//#include "Colourizer.h"
#include "StaticReader.h"

using namespace std;

struct node {
    string node_name;
    string color;
    int id;
    node* next;
};

//id count
int id_count = 0;

//stream to read from readfile
ifstream infile;

//stream to write to writefile
ofstream outfile;

//default graph name
string Graph = "G";

//default server
string server = "http://127.0.0.1:20738/RPC2";

//start of linked list
node* start;

//iterator for linked list of nodes
node* node_ptr;

//the input log file name
const char* readfile = "links.log";

//the output python file name
const char* writefile = "g.py";

/*setup the python file with nessesary libraries, server and graph. Clears
everything from Ubigraph's canvas */
void py_setup(){
    outfile << "import xmlrpclib" << endl;
    outfile << "import time" << endl << endl;   //used for a slower growth ie. time.sleep(6)
    outfile << "server = xmlrpclib.Server('" << server << "')" << endl;
    outfile << Graph << " = server.ubigraph" << endl;
    outfile << Graph << ".clear()" << endl << endl;
}

/* set node_ptr back to start */
void reset_node_ptr(){
    node_ptr = start;
}

/* Takes in a .log file, reads each line and creates two nodes (to node and from node).
Stores the nodes in a linked list, initializes list. DOES NOT ACCOUNT FOR DUPLICATES CURRENTLY */
void parse_file(){
    start = new node;
    start->node_name = "Start: not method";
    start->next = 0;
    start->id = id_count;
    node_ptr = start;
    
    //start reading infile
    string current_line;
    while (getline(infile, current_line)) {
        //get the node names
        string from;
        string to;
        std::istringstream string_stream(current_line);
        string_stream >> from;
        string_stream >> to;
        cout << "from: " << from << " to: " << to << endl;
        
        if(node_ptr != 0 && node_ptr-> next !=0 ){
            while ( node_ptr-> next !=0) {
                cout << "in the loop!" << endl;
                if(node_ptr->node_name != from){
                    node_ptr = node_ptr->next;
                        if( node_ptr-> next == 0 ){
                            node_ptr-> next = new node;     //Create a new from node if not already in list
                            node_ptr = node_ptr-> next;
                            node_ptr->node_name = from;
                            node_ptr->id = ++id_count;
                            node_ptr->next = 0;
                        }
                }
                if(node_ptr->node_name != to){
                    node_ptr = node_ptr->next;
                    if( node_ptr-> next == 0 ){
                        node_ptr-> next = new node;     //Create a new to node if not already in list
                        node_ptr = node_ptr-> next;
                        node_ptr->node_name = from;
                        node_ptr->id = ++id_count;
                        node_ptr->next = 0;
                    }
                }
            }
            reset_node_ptr();
            //cout << node_ptr-> node_name;
            
        }else{
            cout << "at end of list!" << endl;
            node_ptr-> next = new node;     //Create a new from node if not already in list
            node_ptr = node_ptr-> next;
            node_ptr->node_name = from;
            node_ptr->id = ++id_count;
            node_ptr->next = 0;
            reset_node_ptr();
        }
/*
        //create the to and from nodes
        node_ptr-> next = new node;     //The from node
        node_ptr->node_name = from;
        node_ptr->id = id_count;
        outfile << "G.new_vertex_w_id(" << id_count << ")" << endl;     //Create node 1
        id_count++;
        node_ptr = node_ptr-> next;
        node_ptr-> next = 0;
        
        node_ptr-> next = new node;     //The to node
        node_ptr->node_name = to;
        node_ptr->id = id_count;
        outfile << "G.new_vertex_w_id(" << id_count << ")" << endl;     //Create node 2
        
        node_ptr = node_ptr-> next;
        node_ptr-> next = 0;
        //Create the edges
        outfile << "G.new_edge(" << (id_count-1) << "," << id_count << ")" << endl << endl;
        
        id_count++;
*/
    }
    reset_node_ptr();
}

/* iterates through the nodes, starting from wherever the node_ptr is */
void traverse_nodes(){
    if(node_ptr != 0){
        while ( node_ptr-> next !=0) {
            cout << node_ptr->node_name << endl;
            node_ptr = node_ptr->next;
        }
        cout << node_ptr-> node_name;
    }
}

void get_unordered_map(){
	//unordered_map<string, string> sample_RGB = convert_to_RGB(sample, find_max(sample), find_min(sample));
}

int main () {
    
    cout<< "static reader creating unordered map" << endl;
    unordered_map<string, int> funcMap;
    funcMap = generate_ast("/Users/SonikaPrakash/Documents/CPSC*410/fish-shell/proc.cpp", "proc.cpp", funcMap);
    cout << "done creating the map, now printing pairs.." << endl;
    for(auto& entry: funcMap){
		cout << entry.first << " : " << entry.second << endl;
    }
    
    //infile.open(readfile);
    //outfile.open(writefile);
    //py_setup();
    //parse_file();
    //traverse_nodes();
    //infile.close();
    //outfile.close();
  return 0;

}
