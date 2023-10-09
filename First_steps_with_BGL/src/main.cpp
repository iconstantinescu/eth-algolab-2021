/// sample.in
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,                         // no vertex property
  boost::property<boost::edge_weight_t, int>  // interior edge weight property
> WeightedGraph;

typedef boost::graph_traits<WeightedGraph>::edge_iterator EdgeIt;
typedef boost::graph_traits<WeightedGraph>::edge_descriptor EdgeDesc;
typedef boost::property_map<WeightedGraph, boost::edge_weight_t>::type WeightMap;


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  for(int t = 0; t < T; t++) {
    int n, m;
    std::cin >> n >> m;
    WeightedGraph G(n);
    WeightMap weights = boost::get(boost::edge_weight, G);
    
    for(int i = 0; i < m; i++) {
      int u, v, w;
      std::cin >> u >> v >> w;
      EdgeDesc e = boost::add_edge(u, v, G).first; weights[e] = w;
    }
    
    std::vector<EdgeDesc> mst;    // vector to store MST edges (not a property map!)

    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    int edge_sum = 0;

    for (std::vector<EdgeDesc>::iterator it = mst.begin(); it != mst.end(); ++it) 
      edge_sum += weights[*it];
    
    std::cout << edge_sum << " ";
    
    int nv = boost::num_vertices(G);
    std::vector<int> dist_map(nv);
  
    boost::dijkstra_shortest_paths(G, 0,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
        
    int max_dist = -1;
    for(int i = 1; i < nv; i++) {
      if (dist_map[i] > max_dist)
        max_dist = dist_map[i];
    }
  
    std::cout << max_dist << '\n';
    
    
  }
  
  
  return 0;
}