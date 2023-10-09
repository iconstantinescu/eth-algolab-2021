///1
#include<iostream>
#include<set>
#include<vector>
#include<algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  // const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
  //std::cout << matching_size << " " << n ;
  
  // for (int i = 0; i < n; ++i) {
  //   // mate_map[i] != NULL_VERTEX: the vertex is matched
  //   // i < mate_map[i]: visit each edge in the matching only once
  //   if (mate_map[i] != NULL_VERTEX && i < mate_map[i]) {
  //     std::cout << i << " " << mate_map[i] << "\n";
  //     count++;
  //   }
  // }
  
  if(matching_size == n / 2) {
    std::cout << "not optimal" << "\n";
  } else {
    std::cout << "optimal" << "\n";
  }
  
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  for(int t = 0; t < T; t++) {
    int n, c, f;
    std::cin >> n >> c >> f;
    
    std::vector<std::set<std::string>> characteristics(n);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < c; j++) {
        std::string s;
        std::cin >> s;
        characteristics.at(i).insert(s);
      }
      
    }
    
    graph G(n);
    
  
    bool no_common = true;
    for(int i = 0; i < n; i++) {
      
      no_common =  true;
      for(int j = 0; j < n; j++) {
        if(i != j) {
          auto set1 = characteristics.at(i);
          auto set2 = characteristics.at(j);
          std::vector<std::string> intersection;
          std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(intersection));
          int common_char = intersection.size();
          //std::cout << common_char << " ";
          
          if(common_char > f) {
             boost::add_edge(i, j, G);
             no_common = false;
          }
         
        }
      }
      
      if(no_common)
        break;
      
       //std::cout << "\n";
          
    }
    
    if(no_common) {
      std::cout << "optimal" << "\n";
    } else {
       maximum_matching(G);
    }
    
    
   
    
  }
  return 0;
}