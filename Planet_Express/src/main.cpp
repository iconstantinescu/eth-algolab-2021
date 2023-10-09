#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


void testcase(int t) {
  
  int n, m, k, T; std::cin >> n >> m >> k >> T;


  std::set<int> t_vertices;
  for(int i = 0; i < T; i++) {
    int ti; std::cin >> ti;
    t_vertices.insert(ti);
  }

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  
  for(int i = 0; i < m; i++) {
    int from, to, w; std::cin >> from >> to >> w;
    edge_desc e;
    e = boost::add_edge(to, from, G).first; weights[e]=w;
  }


  std::vector<int> component(num_vertices(G)), discover_time(num_vertices(G));
    std::vector< boost::default_color_type > color(num_vertices(G));
    std::vector< vertex_desc > root(num_vertices(G));
  int ncc = boost::strong_components(G,
          boost::make_iterator_property_map(component.begin(), 
      boost::get(boost::vertex_index, G)));


  std::vector<std::vector<int>> t_network(ncc);
  for (auto t : t_vertices) {
    t_network[component[t]].push_back(t);
  }


  for (int ci = 0; ci < ncc; ci++){
    vertex_desc t_center = boost::add_vertex(G);

    for (int j = 0; j < t_network[ci].size(); j++) {
      int from = t_network[ci][j];

      edge_desc e;
      e = boost::add_edge(from, t_center,G).first; 
      weights[e]= 0;

      e = boost::add_edge(t_center, from, G).first; 
      weights[e]= t_network[ci].size() - 1;  
      
    }
  }
  

  int g_size = boost::num_vertices(G);
  std::vector<int> dist_map(g_size);

  boost::dijkstra_shortest_paths(G, n-1,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G))));

  int min_dist = INT32_MAX;

  for(int i = 0; i < k; i++) {
    min_dist = std::min(min_dist, dist_map[i]);
  }


  if(min_dist > 1000000) {
    std::cout << "no" << '\n';
  } else {
    std::cout << min_dist << '\n';
  }

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase(i);
}
