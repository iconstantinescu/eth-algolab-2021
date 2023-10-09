///1
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  
  while(T) {
    
    int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;
    
   
    int start_capacity = 0;
    int finish_capacity = 0;
    std::vector<std::tuple<int, int, int, int>> guides(g);
    for(int i = 0; i < g; i++) {
      int x, y, d, e; std::cin >> x >> y >> d >> e;
      guides[i] = {x, y, d, e};
  
      if(x == k) {
        start_capacity += e;
      }
  
      if(y == a) {
        finish_capacity += e;
      }
    }
    
    int low = 0; int high = std::min(start_capacity, finish_capacity);
    
    while(true) {
      int mid = (low + high + 1) / 2;
      
      // Create graph, edge adder class and propery maps
      graph G(c);
      edge_adder adder(G);  
      
      const vertex_desc v_source = boost::add_vertex(G);
      
      for(const auto[x, y, d, e] : guides) {
        adder.add_edge(x, y, e, d);
      }

      
      adder.add_edge(v_source, k, mid, 0);
      
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      auto rc_map = boost::get(boost::edge_residual_capacity, G);
      

      boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
      
      auto first_edge = *boost::out_edges(boost::vertex(v_source, G), G).first;
      int flow = c_map[first_edge] - rc_map[first_edge];
      int cost = boost::find_flow_cost(G);
      
      // int flow = 0;
      // out_edge_it e, eend;
      // for(boost::tie(e, eend) = boost::out_edges(boost::vertex(k,G), G); e != eend; ++e) {
      //     flow += c_map[*e] - rc_map[*e];     
      // }
      
      if (low == high || (cost <= b && flow < mid)) {
        std::cout << flow << std::endl;
        break;
      }
    
      //std::cout << s_flow << "\n"; 
      
      if(cost > b) {
        high = mid - 1;
      } else {
        low = mid;
      }
    }
    
    T--;
  }
  
    
    
  
  
  return 0;
}