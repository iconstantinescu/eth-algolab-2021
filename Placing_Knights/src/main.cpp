#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;

// Interior Property Maps
typedef  boost::graph_traits<graph>::vertex_descriptor      vertex_desc;
typedef  boost::graph_traits<graph>::edge_descriptor        edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


int graph_idx(int i, int j, int n) {
  return i*n + j;
}

void testcase() {

  int n; std::cin >> n;
  std::vector<std::vector<bool>> present(n, std::vector<bool>(n, 0));
  int count_present = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      bool b; std::cin >> b;
      present[i][j] = b;

      if(present[i][j]) {
        count_present++;
      }
    }  
  }

  graph G(n*n);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  std::vector<int> di = {-1, -1, 1, 1, -2, -2, 2, 2};
  std::vector<int> dj = {-2, 2, -2, 2, -1, 1, -1, 1};

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int g_idx = graph_idx(i,j,n);

      if((i + j) % 2 == 0) {
        if(present[i][j]) {
          adder.add_edge(v_source, g_idx, 1);
        }

        for(int c = 0; c < 8; c++) {
          int next_i = i + di[c];
          int next_j = j + dj[c];
          int g_idx_next = graph_idx(next_i, next_j,n);
          
          if(next_i < n && next_i >= 0 && next_j < n && next_j >=0 && present[i][j]) {
            adder.add_edge(g_idx, g_idx_next, 1);
          }
        }
      } else {
        if(present[i][j]) {
          adder.add_edge(g_idx, v_sink, 1);
        }
      }
      
      
    }  
  }
  

  // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << count_present - flow << "\n";

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
