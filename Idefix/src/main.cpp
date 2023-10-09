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

struct Edge{
  Index u, v;
  K::FT weight;

  Edge(int u, int v, K::FT weight) {
    this->u = u;
    this->v = v;
    this->weight = weight;
  }
};

void testcase() {
  
  Index n, m;
  K::FT s;
  int k;
  std::cin >> n >> m >> s >> k;


  typedef std::pair<K::Point_2,int> IPoint;
  std::vector<IPoint> trees;
  trees.reserve(n);
  for (Index i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    trees.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay triang;
  triang.insert(trees.begin(), trees.end());


  std::vector<std::tuple<K::FT, Index, Index>> distances;
  for(Index i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    Delaunay::Vertex_handle closest_v = triang.nearest_vertex(K::Point_2(x, y));
    distances.push_back({4 * CGAL::squared_distance(closest_v->point(), K::Point_2(x, y)), closest_v->info(), -1});
  }

  for (auto e = triang.finite_edges_begin(); e != triang.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    distances.push_back({triang.segment(e).squared_length(), i1, i2});
  }

  std::sort(distances.begin(), distances.end());

  int a = 0;
  K::FT b = 0;
  bool found = false;
  boost::disjoint_sets_with_storage<> uf(n);
  std::map<Index, int> component_bone_size;
  
  for(auto distance : distances) {
    K::FT squared_dist;
    Index u;
    Index v;
    std::tie(squared_dist, u, v) = distance;

    // if is an edge or a vertex
    if(v == -1) {
      Index cb = uf.find_set(u);
      component_bone_size[cb]++;
    } else {

      Index c1 = uf.find_set(u);
      Index c2 = uf.find_set(v);

      if (c1 != c2) {
        uf.link(c1, c2);
        Index parent = uf.find_set(u);
        component_bone_size[parent] = component_bone_size[c1] + component_bone_size[c2];
      }
    
    }

    if(squared_dist <= s) {
      a = std::max(a, component_bone_size[uf.find_set(u)]);
    }

    if(!found && component_bone_size[uf.find_set(u)] >= k) {
      found = true;
      b = squared_dist;
    }
  }

  std::cout << a << " " << std::setprecision(0) << std::fixed <<  CGAL::to_double(b) << '\n';

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
