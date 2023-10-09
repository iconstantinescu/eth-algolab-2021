#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


double floor_to_double(const CGAL::Quotient<CGAL::Gmpq> &x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

struct Stadium{
  int x;
  int y;
  int d;
  int u;

  Stadium(int x, int y, int d, int u) {
    this->x = x;
    this->y = y;
    this->d = d;
    this->u = u;
  }
};


struct Warehouse{
  int x;
  int y;
  int s;
  int a;

  Warehouse(int x, int y, int s, int a) {
    this->x = x;
    this->y = y;
    this->s = s;
    this->a = a;
  }
};

int var_idx(int i, int j, int m) {
  return i*m + j;
}

void testcase() {
  
  int n, m, c; std::cin >> n >> m >> c;

  std::vector<Warehouse> warehouses; 
  for(int i = 0; i < n; i++) {
    int x, y, s, a; std::cin >> x >> y >> s >> a;
    warehouses.push_back(Warehouse(x,y,s,a));
  }

  std::vector<Stadium> stadiums; 
  for(int i = 0; i < m; i++) {
    int x, y, d, u; std::cin >> x >> y >> d >> u;
    stadiums.push_back(Stadium(x,y,d,u));
  }

  std::vector<std::vector<int>> revenues(n, std::vector<int>(m));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      int r; std::cin >> r;
      revenues[i][j] = r *100;  // SO THAT WE CAN SUBSTRACT 1 FOR CONTOUR LINES
    }
    
  }


  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  

  for(int j = 0; j < m; j++) {
    for(int i = 0; i < n; i++) {
      lp.set_a(var_idx(i, j, m), j,  1);
      lp.set_a(var_idx(i, j, m), m+j,  -1);
      lp.set_a(var_idx(i, j, m), 2*m+j,  warehouses[i].a);
    }


    lp.set_b(j, stadiums[j].d);
    lp.set_b(m+j, -stadiums[j].d);
    lp.set_b(2*m+j, stadiums[j].u * 100);
  }

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      lp.set_a(var_idx(i, j, m), 3*m+i,  1);
      
    }
    lp.set_b(3*m+i, warehouses[i].s);
  }


  std::vector<K::Point_2> pts;
  pts.reserve(n+m);
  for (int i = 0; i < n; ++i) {
    pts.push_back(K::Point_2(warehouses[i].x, warehouses[i].y));
  }

  for (int i = 0; i < m; ++i) {
    pts.push_back(K::Point_2(stadiums[i].x, stadiums[i].y));
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  for(int k = 0; k < c; k++) {
    int x, y, r; std::cin >> x >> y >> r;
    K::Point_2 center = K::Point_2(x,y);

    auto closest_point = t.nearest_vertex(center);

    if(CGAL::squared_distance(closest_point->point(), center) >= CGAL::square(r)) {
      continue;  // IF NO STADIUM OR WAREHOUES IN THIS CIRCLE
    }

    // just check all pairs
    for(int i = 0; i < n; i++) {
      K::Point_2 w = K::Point_2(warehouses[i].x, warehouses[i].y);
      bool warehouse_in = CGAL::squared_distance(w,center) < CGAL::square(r);
      for(int j = 0; j < m; j++) {
        K::Point_2 s = K::Point_2(stadiums[j].x, stadiums[j].y);
        bool stadium_in = CGAL::squared_distance(s,center) < CGAL::square(r);

        if(warehouse_in != stadium_in) {
          revenues[i][j]--;
        }
      }
    }
  }


    for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      lp.set_c(var_idx(i, j, m), -revenues[i][j]);   
    }
  }                  
                                  
                                      
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));

  if(s.is_infeasible()) {
    std::cout << "RIOT!\n";
  } else {
    std::cout << std::setprecision(0) << std::fixed << floor_to_double(-s.objective_value() / 100) << '\n'; 
  }
  
  

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
