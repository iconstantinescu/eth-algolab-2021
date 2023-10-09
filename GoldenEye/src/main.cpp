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

struct Edge {
  Index u, v;
  K::FT sq_dist;

  Edge(Index u, Index v, K::FT sq_dist) {
    this->u = u;
    this->v = v;
    this->sq_dist = sq_dist;
  }

};

void testcase() {
  
  Index n, m;
  K::FT p; 
  std::cin >> n >> m >> p;

  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> jammers;
  jammers.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    jammers.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay triang;
  triang.insert(jammers.begin(), jammers.end());

  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  std::vector<Edge> edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = triang.finite_edges_begin(); e != triang.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(Edge(i1, i2, triang.segment(e).squared_length()));
  }

  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
      return e1.sq_dist < e2.sq_dist;
        });

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf_p(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (std::vector<Edge>::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = uf_p.find_set((*e).u);
    Index c2 = uf_p.find_set((*e).v);
    double sq_len = (*e).sq_dist;

    if (sq_len <= p && c1 != c2) {
    // this edge connects two different components => part of the emst
      uf_p.link(c1, c2);
      //std::cout << *e << "\n";
      if (--n_components == 1) break;
    }

    if(sq_len > p)
      break;
  }


  boost::disjoint_sets_with_storage<> uf_a(n);
  boost::disjoint_sets_with_storage<> uf_b(n);
  K::FT a = 0;
  K::FT b = 0;
  std::vector<Edge>::const_iterator ai = edges.begin();
  std::vector<Edge>::const_iterator bi = edges.begin();

  for (Index i = 0; i < m; ++i) {
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;

    K::Point_2 s = K::Point_2(x0,y0);
    K::Point_2 t = K::Point_2(x1,y1);

    Delaunay::Vertex_handle s_jammer = triang.nearest_vertex(s);
    Delaunay::Vertex_handle t_jammer = triang.nearest_vertex(t);

    
    K::FT s_dist = 4 * CGAL::squared_distance(s_jammer->point(), s);
    K::FT t_dist = 4 * CGAL::squared_distance(t_jammer->point(), t);

    //std::cout << s_jammer << " " << t_jammer;

    Index cs = uf_p.find_set(s_jammer->info());
    Index ct = uf_p.find_set(t_jammer->info());

    if(cs == ct && s_dist <= p && t_dist <= p) {
      std::cout << "y";

      //update uf_b
      b = std::max(b, std::max(s_dist,t_dist));
      while(bi != edges.end() && uf_b.find_set(s_jammer->info()) != uf_b.find_set(t_jammer->info())) {
        uf_b.union_set((*bi).u, (*bi).v);
        
        bi++;
      }


    } else {
      std::cout << "n";
    }
    
    //update uf_a
    a = std::max(a, std::max(s_dist,t_dist));
    while(ai != edges.end() && uf_a.find_set(s_jammer->info()) != uf_a.find_set(t_jammer->info())) {
      uf_a.union_set((*ai).u, (*ai).v);
      ai++;
    }
    
  }

  if(ai != edges.begin()) {
    a = std::max(a, (ai-1)->sq_dist);
  }

  if(bi != edges.begin()) {
    b = std::max(b, (bi-1)->sq_dist);
  }

  std::cout << std::setprecision(0) << std::fixed;
  std::cout << '\n' << a << '\n' << b << '\n';
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
