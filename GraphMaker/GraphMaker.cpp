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
