#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>                  Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>         Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                      Delaunay;

typedef Delaunay::Face_handle Face_handle;
typedef Delaunay::Edge Edge;
typedef K::Point_2 Point;


void testcase(int n) {
  
  std::vector<K::Point_2> infected;
  infected.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    infected.push_back(K::Point_2(x, y));
  }

  
  Delaunay triang;
  triang.insert(infected.begin(), infected.end());


  std::priority_queue<std::pair<K::FT, Face_handle>> queue;

  for(Face_handle face = triang.all_faces_begin(); face != triang.all_faces_end(); face++) {
    if(triang.is_infinite(face)) {
      for(int j = 0; j < 3; j++) {
        Face_handle neighbour = face->neighbor(j);
        K::FT sq_length = triang.segment(Edge(face, j)).squared_length();
        queue.push({sq_length, neighbour});
      }
    } else {
      face->info() = 0;
    }
  }

  while(!queue.empty()) {
    Face_handle face;
    K::FT sq_length;

    std::tie(sq_length, face) = queue.top();
    queue.pop();

    // check if visited
    if(face->info() == 0)
      face->info() = sq_length;

    for(int j = 0; j < 3; j++) {
      Face_handle neighbour = face->neighbor(j);

      if(!triang.is_infinite(neighbour) && neighbour->info() == 0) {
        queue.push({std::min(sq_length, triang.segment(Edge(face, j)).squared_length()), neighbour});
      }
      
    }
  }



  int m; std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int x, y;
    K::FT d;
    std::cin >> x >> y >> d;
    
    Delaunay::Vertex_handle closest_v = triang.nearest_vertex(Point(x,y));
    Face_handle face = triang.locate(Point(x,y));

    if(CGAL::squared_distance(closest_v->point(), Point(x,y)) < d 
      || (!triang.is_infinite(face) && face->info() / 4 < d)) {
      std::cout << 'n';
    } else {
      std::cout << 'y';
    }
  }

  std::cout << "\n";

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;
  while(n != 0) {
    testcase(n);
    std::cin >> n;
  }
  
}
