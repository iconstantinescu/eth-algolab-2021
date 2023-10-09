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

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
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
  
  int e, w, m, d; std::cin >> e >> w >> m >> d;
  int p, l; std::cin >> p >> l;


    graph G(2 * (e + w));
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

  const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

  const vertex_desc v_free_e = boost::add_vertex(G);
    const vertex_desc v_free_w = boost::add_vertex(G);
    

    int free_e = p - l*e;
    int free_w = p - l*w;

    // if(free_e < 0 || free_w < 0) {
    //     std::cout << "No schedule!\n";
    //     return;
    // }

    adder.add_edge(v_source, v_free_e, free_e, 0);
    adder.add_edge(v_free_w, v_sink, free_w, 0);


  for(int i = 0; i < m; i++) {
    int u, v, r; std::cin >> u >> v >> r;
    adder.add_edge(u, 2*e + v, 1, r);
  }

  for(int i = 0; i < d; i++) {
        int u, v, r; std::cin >> u >> v >> r;
    adder.add_edge(e+u, 2*e + w + v, 1, r);
  }


    for(int i = 0; i < e; i++) {
    adder.add_edge(v_source, i, l, 0);
        adder.add_edge(v_free_e, i, free_e, 0);
        adder.add_edge(v_free_e, e+i, free_e, 0);
  }

  for(int i = 0; i < w; i++) {
    adder.add_edge(2*e + i, v_sink, l, 0);
        adder.add_edge(2*e + i, v_free_w, free_w, 0);
        adder.add_edge(2*e + w + i, v_free_w, free_w, 0);
  }
   


    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it eit, eend;
    for(boost::tie(eit, eend) = boost::out_edges(boost::vertex(v_source,G), G); eit != eend; ++eit) {
        // std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
        //     << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
        s_flow += c_map[*eit] - rc_map[*eit];     
    }

    //std::cout << "s-out flow " << s_flow << "\n"; // 5

    if(s_flow < p  || free_e < 0 || free_w < 0) {
        std::cout << "No schedule!\n";
    } else {
        std::cout << cost << "\n";
    }

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
