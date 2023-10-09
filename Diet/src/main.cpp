#include <iostream>
#include <vector>

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


struct food {
  long cost;
  std::vector<long> nutrients;
  
  food(long cost, std::vector<long> nutrients) {
    this->cost = cost;
    this->nutrients = nutrients;
  }
};

int main() {
  
  int n, m;
  std::cin >> n >> m;
  
  while(n != 0 && m != 0) {
    
    std::vector<long> mins;
    std::vector<long> maxes;
    
    for(int i = 0; i < n; i++) {
      long a, b; std::cin >> a >> b;
      mins.push_back(a);
      maxes.push_back(b);
    }
    
    std::vector<food> foods;
    for(int i = 0; i < m; i++) {
      int c; std::cin >> c;
      std::vector<long> nutrients; 
      
      for(int j = 0; j < n; j++) {
        int nutri; std::cin >> nutri;
        nutrients.push_back(nutri);
      }
      
      foods.push_back(food(c, nutrients));
    }
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
         lp.set_a(j, i, foods[j].nutrients[i]);
         lp.set_a(j, n + i, -foods[j].nutrients[i]);
      }
      
      lp.set_b(i, maxes[i]);
      lp.set_b(n+i, -mins[i]);
    }
    
    for(int j = 0; j < m; j++) {
      lp.set_c(j, foods[j].cost);
    }
     
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
  
    
    if(s.is_infeasible()) {
      std::cout << "No such diet.\n";
    } else if(s.is_optimal()) {
      double cost = s.objective_value().numerator().to_double() / s.objective_value().denominator().to_double();
      std::cout << (int) cost << "\n";
    }
    
    
    std::cin >> n >> m;
  }
  
  
  

  
  
  return 0;
}