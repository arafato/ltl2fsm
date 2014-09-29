//#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

using boost::GraphvizDigraph;



template <typename Vertex_Descriptor, typename Vertex_Name_Map>
::std::string const & vertex_label(Vertex_Descriptor v, Vertex_Name_Map & name_map)
{
  return (*(name_map[v].find("label"))).second;
}


// Accepting state?
template <typename Vertex_Descriptor, typename Vertex_Name_Map>
bool accepting(Vertex_Descriptor v, Vertex_Name_Map & name_map)
{
  return (*(name_map[v].find("peripheries"))).second == "2" ? true : false;
}


template <typename Edge_Descriptor, typename Edge_Name_Map>
::std::string const & edge_label(Edge_Descriptor e, Edge_Name_Map & name_map)
{
  return(*(name_map[e].find("label"))).second;
}


void print_all_vertices(GraphvizDigraph const & g)
{
  char c;
  ::std::cout << "VERTICES:\n"; 

  typedef boost::graph_traits<GraphvizDigraph>::vertex_iterator Vertex_Iter_t;

  boost::property_map<GraphvizDigraph, boost::vertex_attribute_t>::const_type
    name_map = boost::get(boost::vertex_attribute, g);

  for(::std::pair<Vertex_Iter_t, Vertex_Iter_t> p = boost::vertices(g); p.first != p.second; ++p.first){
    
    if(accepting(*p.first, name_map))
      c = 'a';
    else
      c = '-';  

    ::std::cout << vertex_label(*p.first, name_map)
                << "[" << c << "]" << ::std::endl;
    
  }
}

void print_all_edges(GraphvizDigraph const & g)
{
  ::std::cout << "EDGES:\n"; 

  typedef boost::graph_traits<GraphvizDigraph>::edge_iterator Edge_Iter_t;

  boost::property_map<GraphvizDigraph, boost::edge_attribute_t>::const_type
    name_map;
  name_map = boost::get(boost::edge_attribute, g);

  for(::std::pair<Edge_Iter_t, Edge_Iter_t> p = boost::edges(g); p.first != p.second; ++p.first){
    ::std::cout << edge_label(*p.first, name_map) << ::std::endl;
  }
}


void scc(GraphvizDigraph const & g)
{
  ::std::cout <<"SCC\n";
  typedef ::boost::graph_traits<GraphvizDigraph>::vertex_descriptor vertex_t;
  ::std::map<vertex_t, int> component;

  int num = strong_components(g, ::boost::make_assoc_property_map(component));

  ::boost::graph_traits<GraphvizDigraph>::vertex_iterator vi, vi_end;
  for(::boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi){
    ::std::cout << component[*vi] << ::std::endl;
  }



//   std::vector<int> c(num_vertices(g));
//   int num = strong_components
//     (g, ::boost::make_iterator_property_map(c.begin(), get(::boost::vertex_index, g)));
  
//   ::std::cout << "Total comps " << num << ::std::endl;
//   ::std::vector<int>::iterator i;
//   for(i = c.begin(); i != c.end(); ++i)
//     ::std::cout << "Vertex " << i - c.begin() << " is in comp " << *i 
//		<< ::std::endl;  
}



int main(int argc, char* argv[]) {

  std::string filename = "graphviz_test.dot";

  if ( argc > 1 )
    filename = argv[1];
  else{
    std::cout << "Usage: " << argv[0] << "  <input>.dot  <output>.dot " << std::endl;
    return 0;
  }
    
  
  ::boost::GraphvizDigraph g;

  boost::read_graphviz(filename, g);

  print_all_vertices(g);
  print_all_edges(g);
  scc(g);

  const char* dot = "graphviz_test_new.dot";
  if ( argc > 2 ) 
    dot = argv[2];

  boost::write_graphviz(dot, g);
  
  return 0;
}
