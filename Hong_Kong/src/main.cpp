#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>


#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef size_t  Index;
typedef CGAL::Triangulation_vertex_base_2<K>                 Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef K::Point_2 Point;
typedef Delaunay::Face_handle Face_handle;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Edge Edge;


void testcase() {
  
  Index n;
  int m;
  K::FT r;
  std::cin >> n >> m >> r;


  std::vector<Point> trees;
  trees.reserve(n);
  for (Index i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    trees.push_back(Point(x, y));
  }

  Delaunay triang;
  triang.insert(trees.begin(), trees.end());

  std::priority_queue<std::pair<K::FT, Face_handle>> queue;

  for(Face_handle face = triang.all_faces_begin(); face != triang.all_faces_end(); face++) {
    if(triang.is_infinite(face)) {
      for(int j = 0; j < 3; j++) {
        Face_handle neighbour = face->neighbor(j);
        K::FT edge_l = triang.segment(Edge(face, j)).squared_length();
        queue.push({edge_l, neighbour});
      }
    } else {
      K::FT circumcenter_rad = CGAL::squared_distance(triang.circumcenter(face), face->vertex(0)->point());
      queue.push({circumcenter_rad, face});
      face->info() = 0;
    }
  }

  while(!queue.empty()) {
    K::FT sq_dist;
    Face_handle face;
    std::tie(sq_dist, face) = queue.top();
    queue.pop();

    if(face->info() == 0) {
      face->info() = sq_dist;
    }

    for(int j = 0; j < 3; j++) {
      Face_handle neighbour = face->neighbor(j);
      if(!triang.is_infinite(neighbour) && neighbour->info() == 0) {
        K::FT edge_l = triang.segment(Edge(face, j)).squared_length();
        queue.push({CGAL::min(sq_dist, edge_l), neighbour});
      }
    }
    
  }


  for(int i = 0; i < m; i++) {
    long x,y;
    K::FT s;
    std::cin >> x >> y >> s;
    Point point = Point(x,y);

    Vertex_handle closest_v = triang.nearest_vertex(point);
    K::FT dist_to_closest = CGAL::squared_distance(point, closest_v->point());
    K::FT minimum_squared_distance = CGAL::square(r + s);

    Face_handle face = triang.locate(point);
    
    if(dist_to_closest < minimum_squared_distance) {
      std::cout << 'n';
    } else {
      if (triang.is_infinite(face) || (face->info() / 4 >= minimum_squared_distance)) {
        std::cout << 'y';
      } else {
        std::cout << 'n';
      }

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
