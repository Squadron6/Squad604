extern "C" {
#include <../UbiGraph/include/UbigraphAPI.h>
}

int Circle(int const argc, const char ** const argv)
{
  for (int i=0; i < 10; ++i)
    ubigraph_new_vertex_w_id(i);

  for (int i=0; i < 10; ++i)
    ubigraph_new_edge(i, (i+1)%10);

  sleep(2);

  ubigraph_clear();
}

#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("graph.py");
  myfile << "import xmlrpclib\n";
  myfile << "server = xmlrpclib.Server('http://127.0.0.1:20738/RPC2')\n";
  myfile << "G = server.ubigraph\n";
  myfile << "G.clear()\n";
  myfile << "x = G.new_vertex()\n";
  myfile << "y = G.new_vertex()\n";
  myfile << "G.new_edge(x,y)\n";
  myfile.close();
  return 0;

}
