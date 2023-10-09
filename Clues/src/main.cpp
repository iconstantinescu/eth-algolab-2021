#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>



typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges


// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef K::Point_2 Point;

void testcase() {
  
  std::size_t n, m;
  K::FT r;
  std::cin >> n >> m >> r;

  std::vector<std::pair<Point, Index>> stations;
  stations.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    stations.push_back({Point(x, y), i});
  }

  std::vector<std::pair<Point, Point>> clues;
  for (std::size_t i = 0; i < m; ++i) {
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;
    
    Point a = Point(x0,y0);
    Point b = Point(x1,y1);
    clues.push_back({a,b});
  }



  Delaunay triang;
  triang.insert(stations.begin(), stations.end());

  graph G(n);
  // build the graph
  for(auto v = triang.finite_vertices_begin(); v != triang.finite_vertices_end(); v++) {
    Delaunay::Vertex_circulator c1 = triang.incident_vertices(v);
    do {
      if(!triang.is_infinite(c1) && CGAL::squared_distance(v->point(), c1->point()) <= r*r) {
        boost::add_edge(v->info(), c1->info(), G);

        auto c2 = triang.incident_vertices(c1);   // just do this trick
        do {
          if (!triang.is_infinite(c2) && v->info() < c2->info()) {
            if (CGAL::squared_distance(v->point(), c2->point()) <= r*r) {
              boost::add_edge(v->info(), c2->info(), G); // from the first to the second to create triangle
            }
          }
        } while (++c2 != triang.incident_vertices(c1));
      }
    } while (++c1 != triang.incident_vertices(v));
  }


  std::vector<bool> results(m, false);
  if(boost::is_bipartite(G)) {

    std::vector<int> component_map(n);  
    int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

    for(int i = 0;  i < clues.size(); i++) {
      Point a = clues[i].first;
      Point b = clues[i].second;

      if(CGAL::squared_distance(a,b) <= r*r) {
        results[i] = true;
      } else {
        Delaunay::Vertex_handle closest_a = triang.nearest_vertex(a);
        Delaunay::Vertex_handle closest_b = triang.nearest_vertex(b);

        K::FT dist_a = CGAL::squared_distance(closest_a->point(), a);
        K::FT dist_b = CGAL::squared_distance(closest_b->point(), b);

        if(dist_a <= r*r && dist_b <= r*r && component_map[closest_a->info()] == component_map[closest_b->info()]) {
          results[i] = true;
        }
      }
    }
    
  }


  for(bool res : results) {
    if(res) {
      std::cout << 'y';
    } else {
      std::cout << 'n';
    }
    
  }

  std::cout << '\n';


  
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
