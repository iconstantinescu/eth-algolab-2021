#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

// Includes
// ========
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
  
  int n, s; std::cin >> n >> s;

  int max_time = 100000;
  int max_cost = 100;

  std::vector<int> cars;
  std::vector<std::vector<int>> s_events(s);
  for(int i = 0; i < s; i++) {
    int l; std::cin >> l;
    cars.push_back(l);
    s_events[i].push_back(0);
    s_events[i].push_back(max_time);
  }


  std::vector<int> price(s);
  std::vector<std::tuple<int,int,int,int,int>> trips;
  for(int i = 0; i < n; i++) {
    int si, ti, di, ai, pi; std::cin >> si >> ti >> di >> ai >> pi;
    si--;
    ti--;
    s_events[si].push_back(di);
    s_events[ti].push_back(ai);
    trips.push_back({si, ti, di, ai, pi});

  }


  std::vector<std::map<int, int>> time_to_idx(s);
  std::vector<int> start_idx(s+1,0);
  for(int i = 0; i < s; i++) {
    std::sort(s_events[i].begin(), s_events[i].end());
    start_idx[i+1] = start_idx[i] + s_events[i].size();

    for(int j = 0; j < s_events[i].size(); j++) {
      time_to_idx[i][s_events[i][j]] = j;
    }
  }

  graph G(start_idx[s]);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  for(int i = 0; i < s; i++) {
    for(int j = 0; j < s_events[i].size() - 1; j++) {
      adder.add_edge(start_idx[i] + j, start_idx[i] + j + 1, INT32_MAX, max_cost*(s_events[i][j+1] - s_events[i][j]));  
    }
  }


  for(int i = 0; i < n; i++) {
    int si, ti, di, ai, pi;
    std::tie(si, ti, di, ai, pi) = trips[i];
    adder.add_edge(start_idx[si] + time_to_idx[si][di], start_idx[ti] + time_to_idx[ti][ai], 1, max_cost*(ai-di)-pi);
  }

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for(int i = 0; i < s; i++) {
    adder.add_edge(v_source, start_idx[i], cars[i], 0);
    adder.add_edge(start_idx[i+1] - 1, v_sink, INT32_MAX, 0);
  }
  

  //Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
     //std::cout << "cost " << cost << "\n"; 
   
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
  
    //std::cout << "s-out flow " << s_flow << "\n"; 

   std::cout << max_cost*max_time*s_flow - cost << "\n"; 


  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
