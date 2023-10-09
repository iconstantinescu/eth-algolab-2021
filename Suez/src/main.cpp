#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct coord{
  int x;
  int y;

  coord(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

double floor_to_double(const CGAL::Quotient<ET> x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a < x) a += 1;
  return a;
}


void testcase() {
  
  int n, m, h, w; std::cin >> n >> m >> h >> w;

  std::vector<coord> free_nails;
  for(int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    free_nails.push_back(coord(x,y));
  }

  std::vector<coord> ocupied_nails;
  for(int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    ocupied_nails.push_back(coord(x,y));
  }


  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  
  //free with free
  int k = 0;
  for(int i = 0; i < n; i++) {
    for(int j = i+1; j < n; j++)  {
      ET dx = std::abs(free_nails[i].x - free_nails[j].x);
      ET dy = std::abs(free_nails[i].y - free_nails[j].y);
      ET cond = 2 * std::max(dx / w, dy / h);

      lp.set_a(i, k,  1); lp.set_a(j, k, 1); lp.set_b(k, cond);  
      k++;
    }
  }


  // free with ocupied
  for(int i = 0; i < n; i++) {

    ET min_cond = INT32_MAX;
    for(int j = 0; j < m; j++)  {
      ET dx = std::abs(free_nails[i].x - ocupied_nails[j].x);
      ET dy = std::abs(free_nails[i].y - ocupied_nails[j].y);
      ET cond = 2 * std::max(dx / w, dy / h) - 1;

      min_cond = std::min(min_cond, cond);

    }

    lp.set_a(i, k,  1); lp.set_b(k, min_cond);  
    k++;
  }
  
  
  
  // objective function (include everything here, otherwise you have rounding errors)
  for(int i = 0; i < n; i++) {
    lp.set_c(i, -2 * (w + h));   
  }
                                      

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  std::cout << std::setprecision( 0 ) 
        << std::fixed 
        << floor_to_double(-s.objective_value()) << '\n'; 
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
