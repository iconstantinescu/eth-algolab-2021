///1
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R; 


int main() {
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  
  while(n != 0) {
    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    R ray = R(P(x,y), P(a,b));
    
    std::vector<S> walls;
    
    for(int i = 0; i < n; i++) {
      long x1,y1,x2,y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      walls.push_back(S(P(x1,y1), P(x2,y2)));
    }
    
    int i = 0;
    while(i < n) {
      if(CGAL::do_intersect(walls[i], ray)) {
        break;
      }
      
      i++;
    }
    
    if(i < n) {
      std::cout << "yes" << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
    
    std::cin >> n;
    
  }
  
  return 0;
}