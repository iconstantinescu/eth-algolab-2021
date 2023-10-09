#include <iostream>
#include <cmath>

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

int64_t round_down(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return static_cast<int64_t>(num);
}


int main() {
  
  int n; std::cin >> n;
  
  while (n != 0) {
    int d; std::cin >> d;
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
    for(int i = 0; i < n; i++) {
      int a; 
      long norm = 0;
      for(int j = 0; j < d; j++) {
        std::cin >> a;
        //std::cout << a << "\n";
        norm += a * a;
        lp.set_a(j, i, a);    
      }
      std::cin >> a;
      norm = std::sqrt(norm);
      //std::cout << norm << "\n";
      lp.set_a(d, i, norm);
      lp.set_b(i, a);
    }
    
    lp.set_l(d, true, 0);
    lp.set_c(d, -1);
    
  
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if(s.is_infeasible()) {
      std::cout << "none\n";
    } else if(s.is_unbounded()) {
      std::cout <<  "inf\n";
    } else if(s.is_optimal()) {
      std::cout << round_down(CGAL::to_double(-s.objective_value())) << std::endl;
    }
    
    // output solution
    //std::cout << s; 
  
    
    std::cin >> n;
  }
  
  
  return 0;
}