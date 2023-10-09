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
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;


void testcase() {
  
  std::size_t m, n;
  std::cin >> m >> n;

  std::vector<std::pair<K::Point_2, int>> participants;
  participants.reserve(m);
  for (std::size_t i = 0; i < m; ++i) {
    int x, y, r;
    std::cin >> x >> y >> r;
    participants.push_back({K::Point_2(x, y), r});
  }

  double h; std::cin >> h;
  std::vector<std::pair<K::Point_2, Index>> lamps;
  lamps.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    lamps.push_back({K::Point_2(x, y),i});
  }


  // construct triangulation
  Delaunay triang;
  triang.insert(lamps.begin(), lamps.end());
  
  std::vector<int> eliminated(m);
  int best_round = 0;
  for (std::size_t i = 0; i < m; ++i) {
    Delaunay::Vertex_handle closest = triang.nearest_vertex(participants[i].first);
    K::FT dist = CGAL::squared_distance(closest->point(), participants[i].first);

    double r = participants[i].second;
    K::FT safe_dist = (h + r) * (h + r);


    if(dist >= safe_dist) {
      eliminated[i] = INT32_MAX;
      best_round = INT32_MAX;
    } else if(best_round != INT32_MAX) {
      for(int  j = 0; j < n; j++) {
        if(CGAL::squared_distance(lamps[j].first, participants[i].first) < safe_dist) {
          eliminated[i] = j;
          best_round = std::max(best_round, j);
          break;
        }
      }

    }
  }

  for(int i = 0; i < m; i++) {
    if(eliminated[i] == best_round)
      std::cout << i << " ";
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
