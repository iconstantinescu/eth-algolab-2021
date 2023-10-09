#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

std::vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  int size = 0;
  for (int i = 0; i < n; ++i) {
    // mate_map[i] != NULL_VERTEX: the vertex is matched
    // i < mate_map[i]: visit each edge in the matching only once
    if (mate_map[i] != NULL_VERTEX && i < mate_map[i]) size++;
  
    //std::cout << i << " " << mate_map[i] << "\n";
  }

  return size;
}


void testcase(int t) {
  
  int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  for(int i = 0; i < m; i++) {
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;

    if(w == 'S') {
      edge_desc e;
      e = boost::add_edge(x, y, G).first; weights[e]=z;
    } else if (w == 'L') {
      edge_desc e;
      e = boost::add_edge(x, y, G).first; weights[e]=z;
      e = boost::add_edge(y, x, G).first; weights[e]=z;
    }
    
  }

  std::vector<std::vector<int>> agents_dist(a);
  for(int i = 0; i < a; i++) {
    int ai; std::cin >> ai;
    agents_dist[i] = dijkstra_dist(G, ai);
  }

  std::vector<int> shelters(s);
  for(int i = 0; i < s; i++) {
    int si; std::cin >> si;
    shelters[i] = si;
  }

  // if(t != 1)
  //   return;


  int low = 0;
  int high = INT32_MAX;

  while(low < high) {
    int mid = low + (high - low) / 2;

    graph BG_mid(a+c*s);
    // Compute BG_mid
    for(int i = 0; i < a; i++) {
      for(int j = 0; j < s; j++) {
        for(int k = 1; k <= c; k++) {
          //std::cout << agents_dist[i][shelters[j]] << "\n";
          if(agents_dist[i][shelters[j]] == INT32_MAX) continue;
          if(agents_dist[i][shelters[j]] + k*d <= mid) {
            boost::add_edge(i, a + (k-1)*s + j, BG_mid);
          }
        }
      }
    }

    int matching_size = maximum_matching(BG_mid);
    //std::cout << matching_size << "\n";

    if(matching_size == a) {
      high = mid;
    } else if (matching_size < a) {
      low = mid + 1;
    } 

  }

  std::cout << low << '\n';


  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase(i);
}
