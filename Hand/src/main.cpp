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
  K::FT sq_dist;

  Edge(Index u, Index v, K::FT sq_dist) {
    this->u = u;
    this->v = v;
    this->sq_dist = sq_dist;
  }
};

int can_host(int k, std::vector<int> &count_by_size) {
  int res = count_by_size[k];

  if(k == 4) {
    res += std::min(count_by_size[1], count_by_size[3]);
    int remaining3 = count_by_size[3] - std::min(count_by_size[1], count_by_size[3]);
    int remaining1 = count_by_size[1] - std::min(count_by_size[1], count_by_size[3]);
    res += (count_by_size[2] +  remaining3) / 2;  // 2 with 3 or 2 with 2 or 3 with 3 is the same, all work
    res += (((count_by_size[2] +  remaining3) % 2) * 2  + remaining1) / 4; // count 2 as 2 ones
  } else if(k == 3) {
    res += std::min(count_by_size[1], count_by_size[2]);
    res += (count_by_size[1] - std::min(count_by_size[1], count_by_size[2])) / 3;
    res += (count_by_size[2] - std::min(count_by_size[1], count_by_size[2])) / 2; 
  } else if(k == 2) {
    res += count_by_size[1] / 2;
  }


  return res;
}

void testcase() {
  
  Index n;
  int k, f0;
  K::FT s0;
  std::cin >> n >> k >> f0 >> s0;

  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> tents;
  tents.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    tents.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay triang;
  triang.insert(tents.begin(), tents.end());

  std::vector<Edge> edges;
  edges.reserve(3*n); 
  for (auto e = triang.finite_edges_begin(); e != triang.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, triang.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
      return e1.sq_dist < e2.sq_dist;
        });

  //std::cout << "here";
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  std::vector<int> comp_size(n, 1);
  std::vector<int> count_by_size(k+1);
  count_by_size[1] = n;
  K::FT max_length = 0;
  int max_families = 1;
  for (std::vector<Edge>::iterator e = edges.begin(); e != edges.end(); ++e) {

    int can_host_now = can_host(k, count_by_size);
    //std::cout << e->sq_dist << " " << count_by_size[1] << '\n';
    if(can_host_now >= f0) {
      max_length = e->sq_dist;
    }

    if(e->sq_dist >= s0) {
      max_families = std::max(max_families, can_host_now);
    }
    
    Index cu = uf.find_set(e->u);
    Index cv = uf.find_set(e->v);
    if (cu != cv) {

      count_by_size[std::min(k, comp_size[cu])]--;
      count_by_size[std::min(k, comp_size[cv])]--;

      uf.link(cu, cv);
      Index parent = uf.find_set(e->u);
      comp_size[parent] = comp_size[cu] + comp_size[cv];
      count_by_size[std::min(k, comp_size[parent])]++;
      // std::cout << e->u << "\n";
      //if (--n_components == 1) break;
      
    }

  }

  //std::cout << k << '\n';
  std::cout << std::setprecision(0) << std::fixed << CGAL::to_double(max_length) << " " << max_families << '\n';
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
