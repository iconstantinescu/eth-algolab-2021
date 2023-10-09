#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/pending/disjoint_sets.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

struct Edge {
  int u, v;
  int weight;

  Edge(int u, int v, int weight) {
    this-> u= u;
    this-> v = v;
    this->weight = weight;
  }
};

void testcase() {
  
  int n, i; std::cin >> n >> i;

  std::vector<Edge> edges;
  for(int i = 0; i < n- 1; i++) {
    for(int j = 1; j < n - i; j++) {
      int c; std::cin >> c; 
      edges.push_back(Edge(i, i + j, c));
    }
  }

  std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
        return e1.weight < e2.weight;
            });


   // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  std::vector<Edge> mst_edges;
  // ... and process edges in order of increasing length
  for (auto e = edges.begin(); e != edges.end(); ++e) {
    
    // determine components of endpoints
    int cu = uf.find_set(e->u);
    int cv = uf.find_set(e->v);
    if (cu != cv) {
      // this edge connects two different components => part of the emst
      uf.link(cu, cv);
      mst_edges.push_back(*e);
      //std::cout << *e << "\n";
      if (--n_components == 1) break;
    }
  }

  int min_cost = INT32_MAX;
  for(Edge restricted_edge : mst_edges) {
    int sum = 0;
    boost::disjoint_sets_with_storage<> ufr(n);
    int n_components = n;
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      if(!(e-> u == restricted_edge.u && e->v == restricted_edge.v)) {
        // determine components of endpoints
        int cu = ufr.find_set(e->u);
        int cv = ufr.find_set(e->v);
        if (cu != cv) {
          // this edge connects two different components => part of the emst
          ufr.link(cu, cv);
          sum += e->weight;
          //std::cout << *e << "\n";
          if (--n_components == 1) break;
        }
      }
    }

    if(sum < min_cost) {
      min_cost = sum;
    }
  }

  std::cout << min_cost << '\n';


  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
