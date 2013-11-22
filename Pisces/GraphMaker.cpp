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

using namespace std;

struct node  {
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
const char* readfile = "astLog.txt";

//the output python file name
const char* writefile = "graph.py";

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

/* iterates through the nodes, starting from wherever the node_ptr is add to python file*/
void create_nodes(){
       if(node_ptr != NULL){
           cout << "node ptr not null..start traversing" << endl;
        while ( node_ptr-> next != NULL ) {
            cout << "node name: " << node_ptr->node_name << endl;
           
            //add node to python file
            if (node_ptr->id > -1) { //make sure not to include start node
                outfile << "G.new_vertex_w_id(" << node_ptr->id << ")" << endl;
                outfile << "G.set_vertex_attribute(" << node_ptr->id << "," << "'color', '" << node_ptr-> color << "')" << endl;
                outfile << "G.set_vertex_attribute(" << node_ptr->id << ",'shape', 'sphere')" << endl;
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

void create_edges(){
    cout << "**reading the astLog.txt here is what i found: " << endl;
    string current_line;
    while (getline(infile, current_line)) {
        //get the node names
        string from;
        string to;
        std::istringstream string_stream(current_line);
        string_stream >> from;
        string_stream >> to;
        cout << "**from: " << from << " to: " << to << endl;
        
        if(node_ptr != NULL){
            cout << "finding the node..." << endl;
            while ( node_ptr-> next != NULL ) {
                if( node_ptr->node_name == from ){
                    cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and from is: " << from << endl;
                    
                    
                }
                node_ptr = node_ptr->next;
            }
            //make sure to take care of the last node here
            if( node_ptr->node_name == from ){
                cout << "@@@ match found! node_ptr->name is: " << node_ptr->node_name << " and from is: " << from << endl;
            }
        }
      reset_node_ptr();
    }
    
}


int main () {
    
    cout<< "**static reader creating unordered map" << endl;
    unordered_map<string, int> funcMap;
    funcMap = generate_ast("/Users/SonikaPrakash/Documents/CPSC*410/fish-shell/proc.cpp", "proc.cpp", funcMap);
    cout << "**done creating the map, now printing pairs.." <<endl;
    
    std::cout << "**funcMap contains:";
    for ( auto it = funcMap.begin(); it != funcMap.end(); ++it )
        std::cout << " " << it->first << ":" << it->second;
    std::cout << endl << endl;
    
    /*
    //Dummy map
    unordered_map <string, int> dummy;
    dummy["test1"] = 25;
    dummy["test2"] = 1;
    dummy["test3"] = 50;
    dummy["test4"] = 1;
    dummy["test5"] = 100;
    dummy["test6"] = 1;
    dummy["test7"] = 1;
    dummy["test8"] = 75;
    dummy["test9"] = 0;
    dummy["test10"] = 1;
    
    //This doesn not print anything when funcMap is passed in, I'm assuming the reason is that funcMap does not get filled by generate_ast, so I'm passing in a dummy map for now
    std::cout << "**dummy map contains:";
    for ( auto it = dummy.begin(); it != dummy.end(); ++it )
        std::cout << " " << it->first << ":" << it->second;
    std::cout << endl << endl;
    */
     
    //Create the Colorizer hash map (again passing in dummy map, since funcMap doesnt seem to work)
    unordered_map<string, string> colored_map = convert_to_RGB(funcMap, find_max(funcMap), find_min(funcMap));
   
    /* Contents of colored_map (this does work) */
    //std::cout << "colored_map contains:";
    for ( auto it = colored_map.begin(); it != colored_map.end(); ++it )
        //std::cout << " " << it->first << ":" << it->second;
    //std::cout << std::endl;
    
    //initialize linked list
    start = new node;
    start->node_name = "start";
    start->color = " ";
    start->id = -1;
    start->next = NULL;
    node_ptr = start;
    cout << "**Creating linked list, the start node: " << node_ptr->node_name <<endl;
    
    create_nodes(colored_map);
    
    //start creating python script for ubigraph
    infile.open(readfile);      //use the log file to generate edges
    outfile.open(writefile);    //this is the output python file
    
    cout << "creating python script" << endl;
    py_setup();
    //sanity check: have nodes been created?
    
    cout << "sanity check: can we traverse nodes?" << endl;
    create_nodes();
    
    //create edges between nodes
    create_edges();
    
    outfile.close();
    cout << "finished creating python script" << endl;
    infile.close();
    
    
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