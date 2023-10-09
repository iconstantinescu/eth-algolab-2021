///4
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>


// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

std::vector<edge_desc> kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  return mst;

//   for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
//     std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
//   }
}

long dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<long> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void testcase() {
  
  int n, e, s, a, b; std::cin >> n >> e >> s >> a >> b;

  typedef std::pair<int, int> edge_pair;
  std::map<edge_pair, int> edge_weight;
  std::vector<weighted_graph> s_graphs;
  std::vector<weight_map> s_weights;

  for(int j = 0; j < s; j++) {
    weighted_graph g(n);
    s_graphs.push_back(g);
    s_weights.push_back(boost::get(boost::edge_weight, g));
  }
  

  weighted_graph G;
  weight_map G_weights = boost::get(boost::edge_weight, G);

  for(int i = 0; i < e; i++) {
    int from, to; std::cin >> from >> to;
    // if (from > to) {
    //     std::swap(from, to);
    // }

    int min_weight = INT_MAX;
    int w;
    for(int j = 0; j < s; j++) {
      std::cin >> w;
      edge_desc e;
      e = boost::add_edge(from, to, s_graphs[j]).first, s_weights[j][e]= w;
    }

    edge_weight[{from, to}] = min_weight;
  }

  std::set<edge_pair> network_edges;
  for(int j = 0; j < s; j++) {
    int h; std::cin >> h;
    std::vector<edge_desc> s_mst = kruskal(s_graphs[j]);
    for(auto e : s_mst) {
      int from = boost::source(e, s_graphs[j]);
      int to = boost::target(e, s_graphs[j]);
      // if (from > to) {
      //     std::swap(from, to);
      // }
        
      network_edges.insert({from, to});
      
      if(edge_weight.find({from, to}) == edge_weight.end()) {
        edge_weight[{from, to}] = s_weights[j][e];
      } else {
        edge_weight[{from, to}] = std::min(edge_weight[{from, to}], s_weights[j][e]);
      }
    }
  }

  for(auto e_pair : network_edges) {
    edge_desc e;
    e = boost::add_edge(e_pair.first, e_pair.second, G).first, G_weights[e]= edge_weight[{e_pair.first, e_pair.second}];
  }


  std::cout << dijkstra_dist(G, a, b) << std::endl;

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
