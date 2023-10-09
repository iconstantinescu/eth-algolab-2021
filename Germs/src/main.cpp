#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while(n != 0) {
    
    int l, b, r, t; std::cin >> l >> b >> r >> t;
    
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for(int i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      pts.push_back(K::Point_2(x, y));
    }
    
    Triangulation triang;
    triang.insert(pts.begin(), pts.end());
    
    std::vector<double> min_dists;
    min_dists.reserve(n);
    for(auto v = triang.finite_vertices_begin(); v != triang.finite_vertices_end(); v++) {
      const int x = v->point().x() ;
      const int y = v->point().y();
      
      double dy = std::min(t - y, y - b);
      double dx = std::min(r - x, x - l);
      double min_dist = std::min(dx,dy); // min distance to boundries
      
      Triangulation::Vertex_circulator c = triang.incident_vertices(v);
      if (c != 0) {
        do {
          if (!triang.is_infinite(c)) {
            min_dist = std::min(min_dist, std::sqrt(CGAL::squared_distance(v->point(), c->point())) / 2);
          }
      
        } while (++c != triang.incident_vertices(v));
      }
      

      min_dists.push_back(min_dist);
    }
    
    std::sort(min_dists.begin(), min_dists.end());
    int first = std::ceil(std::sqrt(min_dists[0] - 0.5));
    int middle = std::ceil(std::sqrt(min_dists[n/2] - 0.5));
    int last = std::ceil(std::sqrt(min_dists[n-1] - 0.5));
    
    std::cout << first << " " <<  middle << " " << last << '\n';
    
    std::cin >> n;
  }
  
  return 0;
}