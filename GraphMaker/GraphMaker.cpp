#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <Colourizer.h>

using namespace std;

struct node {
    string node_name;
    string color;
    node* next;
};

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
    outfile << Graph << ".clear()" << endl;
}

/* set node_ptr back to start */
void reset_node_ptr(){
    node_ptr = start;
}

/* Takes in a .log file, reads each line and creates two nodes (to node and from node).
Stores the nodes in a linked list, initializes list. */
void parse_file(){
    start = new node;
    start->node_name = "Start: not method";
    start->next = 0;
    node_ptr = start;
    
    //start reading infile
    string current_line;
    while (getline(infile, current_line)) {
        //getline(infile, current_line);
        //cout << current_line << endl;
        
        //get the node names
        string from;
        string to;
        std::istringstream string_stream(current_line);
        string_stream >> from;
        string_stream >> to;
        
        //create the to and from nodes
        node_ptr-> next = new node;     //The from node
        node_ptr->node_name = from;
        node_ptr = node_ptr-> next;
        node_ptr-> next = 0;
     
        node_ptr-> next = new node;     //The to node
        node_ptr->node_name = to;
        node_ptr = node_ptr-> next;
        node_ptr-> next = 0;
        //cout << "from: " << from_node->node_name << " to: " << to_node->node_name << endl;
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

int main () {
    
    infile.open(readfile);
    outfile.open(writefile);
    py_setup();
    parse_file();
    traverse_nodes();
    infile.close();
    outfile.close();
  return 0;

}
