///
#include <iostream> 
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

long floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a < x) a += 1;
  return a;
}



int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while(n != 0) {
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for(int i = 0 ; i < n; i++) {
      int x, y; std::cin >> x >> y;
      pts.push_back(K::Point_2(x, y));
      
    }
    
     // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    int m; std::cin >> m;
    for(int i = 0; i < m; i++) {
      int x, y; std::cin >> x >> y;
      K::Point_2 test_pt = K::Point_2(x, y);
      K::Point_2 closest_pt = t.nearest_vertex(K::Point_2(x, y))->point();
      
      std::cout << floor_to_double(CGAL::squared_distance(test_pt, closest_pt)) << '\n';
    }
    
     std::cin >> n;
  }
  
  return 0;
}