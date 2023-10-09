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
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_double(const CGAL::Quotient<CGAL::Gmpz>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long) a;
}


void testcase() {
  
  long xs, ys;
  int n; 
  std::cin >> xs >> ys >> n;

  const int T = 0; 
  const int X = 1; 
  const int Y = 2; 
  Program lp (CGAL::SMALLER, false, 0, false, 0); 

  for(int i = 0; i < n; i++) {
    long a, b, c, v; std::cin >> a >> b >> c >> v;
    const int norm = std::sqrt(a*a + b*b);

    if(a * xs + b * ys + c > 0) {
      lp.set_a(X, i,  -a); lp.set_a(Y, i, -b); lp.set_a(T, i, norm*v); lp.set_b(i, c);  
    } else {
      lp.set_a(X, i,  a); lp.set_a(Y, i, b); lp.set_a(T, i, norm*v); lp.set_b(i, -c);  
    }                        

  }

  lp.set_l(T, 0);
  lp.set_c(T, -1);               
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  std::cout << floor_to_double(-s.objective_value()) << '\n'; 

  
  

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
