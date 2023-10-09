#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
  boost::property<boost::edge_residual_capacity_t, long,
  boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
  
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
  public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity) {
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  const auto e = boost::add_edge(from, to, G).first;
  const auto rev_e = boost::add_edge(to, from, G).first;
  c_map[e] = capacity;
  c_map[rev_e] = 0; // reverse edge has no capacity!
  r_map[e] = rev_e;
  r_map[rev_e] = e;
  }
};


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int l, p; std::cin >> l >> p;
    
    graph G(l);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    std::vector<int> demands(l);
    
    for(int i = 0; i < l; i++) {
      int g, d; std::cin >> g >> d;
      demands.at(i) = d - g;
    }
    
    for(int i = 0; i < p; i++) {
      int from, to, cmin, cmax; std::cin >> from >> to >> cmin >> cmax;
      demands.at(from) += cmin;
      demands.at(to) -= cmin;
      adder.add_edge(from, to, cmax - cmin);
    }
    
    int total_demand = 0;
    for(int i = 0; i < l; i++) {
      if(demands.at(i) < 0) {
        adder.add_edge(v_source, i, -demands.at(i));
      } else if(demands.at(i) > 0) {
        total_demand += demands.at(i);
        adder.add_edge(i, v_sink, demands.at(i));
      }
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    if(flow == total_demand) {
      std::cout << "yes\n";
    } else {
      std::cout << "no\n";
    }

    
    T--;
  }
  
  return 0;
}