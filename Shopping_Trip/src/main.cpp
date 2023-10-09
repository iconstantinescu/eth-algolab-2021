#include <iostream>

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

// Custom edge adder class, highly recommended
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
    
    int n, m, s; std::cin >> n >> m >> s;
    
     graph G(n);
    
    const vertex_desc v_source = *boost::vertices(G).first;
    const vertex_desc v_sink = boost::add_vertex(G);
  
    edge_adder adder(G);
    
    std::vector<int> stores;
    for(int i = 0; i < s; i++) {
      int l; std::cin >> l;
      stores.push_back(l);
      adder.add_edge(l, n, 1);
    }
    
    for(int i = 0; i < m; i++) {
      int from, to; std::cin >> from >> to;
      adder.add_edge(from, to, 1);
      adder.add_edge(to, from, 1);
    }
    
    int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    if(flow == stores.size()) {
      std::cout << "yes\n";
    } else {
      std::cout << "no\n";
    }
    
    T--;
  }
  
  return 0;
}