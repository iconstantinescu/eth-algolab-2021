#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef K::Point_2 P;

long to_long(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long) a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n != 0) {
    long a,b,x,y;
    std::cin >> a >> b >> x >> y;
    R ray = R(P(a,b), P(x,y));
    P best_point;
    K::FT best_dist;
    bool found_intersection = false;
    
    for(int i = 0; i < n; i++) {
      long x1,y1,x2,y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      S wall = S(P(x1,y1), P(x2,y2));
      if(CGAL::do_intersect(wall, ray)) {
        auto o = CGAL::intersection(wall,ray);
        P intersection_point;
        if (const P* op = boost::get<P>(&*o)) {
          intersection_point = *op;
        }
        else if (const S* os = boost::get<S>(&*o)) {
          if(CGAL::squared_distance(os->source(), ray.source()) 
              < CGAL::squared_distance(os->target(), ray.source()))
            intersection_point = os->source();
          else 
            intersection_point = os->target();
        }
        
        K::FT cur_dist = CGAL::squared_distance(intersection_point, ray.source());
                     
        if(found_intersection == false) {
          found_intersection = true;
          best_point = intersection_point;
          best_dist = cur_dist;
        }
        else if(cur_dist < best_dist) {
          best_point = intersection_point;
          best_dist = cur_dist;
        }
      }
    }
    
    if(found_intersection == true) 
      std::cout << to_long(best_point.x()) << " " 
                << to_long(best_point.y())  << '\n';
    else
      std::cout << "no" << std::endl;
    
    std::cin >> n;
  }
  
  //std::cout << std::endl;
  return 0;
}