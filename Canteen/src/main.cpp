#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <iostream>
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
  
  int n; std::cin >> n;

  graph G(n);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

  const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

  // Read the data
  std::vector<std::pair<int, int>> supply;
  for(int i = 0; i < n; i++) {
    int ai, ci; std::cin >> ai >> ci;
    supply.push_back({ai, ci});
  }

  int max_price = 0;
  int sum_demand = 0;
  std::vector<std::pair<int, int>> demand;
  for(int i = 0; i < n; i++) {
    int si, pi; std::cin >> si >> pi;
    demand.push_back({si, pi});
    max_price = std::max(max_price, pi);
    sum_demand += si;
  }


  std::vector<std::pair<int, int>> freeze;
  for(int i = 0; i < n - 1; i++) {
    int vi, ei; std::cin >> vi >> ei;
    freeze.push_back({vi, ei});
  }


  for(int i = 0; i < n; i++) {
    adder.add_edge(v_source, i, supply[i].first, supply[i].second);
    adder.add_edge(i, v_sink, demand[i].first, -demand[i].second + max_price);
    if(i < n - 1) {
      adder.add_edge(i, i + 1, freeze[i].first, freeze[i].second);
    }
  }

  
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int flow_cost = boost::find_flow_cost(G);
    // std::cout << "-----------------------" << std::endl;
    // std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << "\n";
    // std::cout << "cost " << cost2 << "\n"; 


    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        // std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
        //     << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
        s_flow += c_map[*e] - rc_map[*e];     
    }

  if(s_flow == sum_demand) {
    std::cout << "possible ";
  } else {
    std::cout << "impossible ";
  }

    std::cout << s_flow << " " << -flow_cost + max_price * s_flow << "\n"; 

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
