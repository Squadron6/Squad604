#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "CodeInjector.h"
#include "Colourizer.h"
#include "StaticReader.h"
#include "DynamicReader.h"
#include "GraphMaker.h"

using namespace std;

/**
    main.cpp ties together all the internal components of Pisces and produces a python file that Ubigraph will use to create the graph
    @author Sonika Prakash
 
 */

int main (int argc, char* argv[]) {
    
    // IO stream to read from
    ifstream infile;
    // IO stream to write to
    ofstream outfile;
    // The directory that contains the source files of the code base
    string dir = "../codeBase/fish-shell-master/";

    /* Set up for Dynamic analysis:
      1. Fish needs to have log files injected into it
      2. Fish then is configured, compiled, and installed
      3. Fish is run and waits for user input
      4. Once the user exits Fish, log.txt is created
    */
    string fishauto = "(cd " + dirname + " && autoconf)"; 
    string fishconf = "(cd " + dirname + " && ./configure --prefix=$HOME)";
    string fishmake = "make install -C " + dirname;
    string fishrun = "fish";
    exploreInjectDirectory(dirname);
    system(fishauto.c_str());
    system(fishconf.c_str());
    system(fishmake.c_str());
    system(fishrun.c_str());

    // Log file that contains dynamic output results (a list of the names of functions)
    const char* readfile_dynamic = "log.txt";
    // Log file where the static AST is stored
    const char* readfile = "astLog.txt";
    // The python file that will be generated (passed into UbiGraph as input)
    const char* writefile = "graph.py";
    
    /* Creates the first node in the linked list. Each element in the linked list represents a node in the graph. Each element contains the node name, color and id. */
    initial_linked_list();
    
    /* Create the unordered map that will contain the static function names as keys and frequency of funtions as values. */
    unordered_map<string, int> funcMap;
    // Fills the unordered map
    funcMap = exploreDirectory(dir);
     
    /* Create another unordered map (based on the funMap) that will contain the static function names as keys and function colors as values */
    unordered_map<string, string> colored_map = convert_to_RGB(funcMap, find_max(funcMap), find_min(funcMap));
    
    /* Creates a linked list of nodes (in no particular order, nodes are just created and added to the linked list) */
    create_linked_list_nodes(colored_map);
    
    /* Incase we don't start at the beginning of the node list, reset the node pointer for future use. */
    reset_node_ptr();
    
    /* Create unordered map that will contain colors of functions based on dynamic function frequency results */
    unordered_map<string, int> funcMap2;
    funcMap2 = parse_log(readfile_dynamic);
    unordered_map<string, string> colored_map_dynamic = convert_to_RGB(funcMap2, find_max(funcMap2), find_min(funcMap2));
    
    /* Start creating python script as input for UbiGraph */
    infile.open(readfile);
    outfile.open(writefile);
    
    /* Adds libraries and other setup stuff to python file */
    py_setup(outfile);
    
    /* Iterate through the linked list and create the nodes with their respective colors in the python file */
    create_nodes_python(outfile);
    
    /* Create edges between nodes bases on the from:to pairs in the log file in the python file */
    create_edges(infile,outfile);
    
    /* Add the dynamic runtime function frequency color to the nodes from the static graph */
    recolor_nodes(colored_map_dynamic, outfile);
    
    /* Finished creating the python, close all IO streams that were opened */
    outfile.close();
    infile.close();
    
    return 0;

}
