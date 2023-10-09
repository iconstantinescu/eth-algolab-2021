#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
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

void testcase() {
  
  int n, m, s; std::cin >> n >> m >> s;

  graph G(s + n + m);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

  const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

  for(int i = 0; i < s; i++) {
    int li; std::cin >> li;
    adder.add_edge(v_source, i, li, 0);
  }

  for(int i = 0; i < m; i++) {
    int si; std::cin >> si;
    adder.add_edge(si - 1, s + i, 1, 0);
  }

  int max_cost = 0;
  std::vector<std::vector<int>> cost(n, std::vector<int>(m)); 
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      int c_ij; std::cin >> c_ij;
      cost[i][j] = c_ij;
      max_cost = std::max(max_cost, c_ij);
    }
  }


  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      adder.add_edge(s + j, s + m + i, 1, max_cost - cost[i][j]);
    }
  }


  for(int i = 0; i < n; i++) {
    adder.add_edge(s + m + i, v_sink, 1, 0);
  }


  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int flow_cost = boost::find_flow_cost(G);
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << "\n";
    //std::cout << "cost " << flow_cost << "\n"; 
    // Iterate over all edges leaving the source to sum up the flow values.

    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        // std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
        //     << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
        s_flow += c_map[*e] - rc_map[*e];     
    }
    std::cout << s_flow << " " << -flow_cost + max_cost * s_flow << "\n"; 
    

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
