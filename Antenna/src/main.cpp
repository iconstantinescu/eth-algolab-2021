#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;

long to_long(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a < x) a += 1;
  return (long) a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n != 0) {
    std::vector<P> locations;
    for(int i = 0; i < n; i++) {
      long x,y;
      std::cin >> x >> y;
      locations.push_back(P(x,y));
    }
    
    
    Min_circle mc(locations.begin(), locations.end(), true);
    Traits::Circle c = mc.circle();
    
    std::cout << to_long(CGAL::sqrt(c.squared_radius())) << std::endl;
    std::cin >> n;
  }
  
  return 0;
}