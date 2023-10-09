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

void solve_p1(int a, int b) {
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  const int X = 0; 
  const int Y = 1;
  
  lp.set_a(X, 0, 1);  lp.set_a(Y, 0, 1); lp.set_b(0, 4); 
  lp.set_a(X, 1, 4);  lp.set_a(Y, 1, 2); lp.set_b(1, a * b); 
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); 
  
  
  lp.set_c(X, a);   
  lp.set_c(Y, -b);    
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  //std::cout << s; 
  if (s.is_optimal()) {
    CGAL::Quotient<ET> obj_val = s.objective_value();
    double res = -obj_val.numerator().to_double() / obj_val.denominator().to_double();
    std::cout << (int) std::floor(res) << '\n';
    
  } else if (s.is_unbounded()) {
    std::cout << "unbounded" << '\n';
    
  } else if (s.is_infeasible()) {
    std::cout << "no" << '\n'; 
  }
  
}

void solve_p2(int a, int b) {
  Program lp (CGAL::LARGER, false, 0, true, 0);
  
  const int X = 0; 
  const int Y = 1;
  const int Z = 2;
  
  lp.set_a(X, 0, 1);  lp.set_a(Y, 0, 1); lp.set_b(0, -4); 
  lp.set_a(X, 1, 4);  lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b); 
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1); 
  
  
  lp.set_c(X, a);   
  lp.set_c(Y, b);
  lp.set_c(Z, 1); 
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  if (s.is_optimal()) {
    CGAL::Quotient<ET> obj_val = s.objective_value();
    double res = obj_val.numerator().to_double() / obj_val.denominator().to_double();
    
    std::cout << (int) std::ceil(res) << '\n';
    
  } else if (s.is_unbounded()) {
    std::cout << "unbounded" << '\n';
    
  } else if (s.is_infeasible()) {
    std::cout << "no" << '\n'; 
  }
  
  // output solution
  //std::cout << s; 

}


int main() {

  int p, a, b;
  std::cin >> p;
  
  while(p != 0) {
    
    std::cin >> a >> b;
    
    if(p == 1) {
      solve_p1(a, b);
    } else {
      solve_p2(a, b);
    }
    
    std :: cin >> p;
  }
  
  return 0;
}