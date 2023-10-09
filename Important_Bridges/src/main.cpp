///1
#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>
#include <iostream>
#include <algorithm>

      
namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, 
        boost::no_property,
        boost::property< boost::edge_component_t,  std::size_t >> graph;
  
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::edge_descriptor egde_desc;

typedef boost::property_map<graph, boost::edge_component_t>::type edge_components;


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  for(int t = 0; t < T; t++) {
    int n, m;
    std::cin >> n >> m;
    graph G(n);
    
    for(int i = 0; i < m; i++) {
      int u, v;
      std::cin >> u >> v;
      boost::add_edge(u, v, G);
    }
    
    edge_components components = boost::get(boost::edge_component, G);
    std::size_t n_comp = boost::biconnected_components(G, components);
    std::vector<std::vector<egde_desc>> comp_edges(n_comp);
    
    
    edge_it ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
      comp_edges.at(components[*ei]).push_back(*ei);
    }
    
    std::vector<std::pair<int, int>> critical_bridges;
    
    for(auto it = comp_edges.cbegin(); it != comp_edges.cend(); it++) {
      if((*it).size() == 1) {
        int first = std::min(boost::source((*it).at(0), G), boost::target((*it).at(0), G));
        int second = std::max(boost::source((*it).at(0), G), boost::target((*it).at(0), G));
        critical_bridges.push_back(std::make_pair(first,second));
      }
        
    }
    
    std::sort(critical_bridges.begin(), critical_bridges.end());
    std::cout << critical_bridges.size() << '\n';

    for(auto it = critical_bridges.cbegin(); it != critical_bridges.cend(); it++) {
      std::cout << std::min((*it).first, (*it).second) << " " << std::max((*it).first, (*it).second) << '\n';
    }
    
    
    
  }
  
  
  
  return 0;
}