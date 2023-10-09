#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef std::pair<int, int> P;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long round_up(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num < input_val) num += 1;
  return (long) num;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  
  while(T)  {
    int n, m; long s; 
    std::cin >> n >> m >> s;
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
     // set the coefficients of A and b
    const int a = 0; 
    const int b = 1;
    const int c = 2;
    const int d = 3;
    const int z = 4;
    
    long sum_x = 0;
    long sum_y = 0;
    
    std::vector<P> points;
    for (int i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      points.push_back(std::make_pair(x,y));
      
      // noble houses are on the left
      sum_x += -x;
      sum_y += -y;
      
      // ax+by+c <= 0
      lp.set_a(a, i, x);
      lp.set_a(b, i, y);
      lp.set_a(c, i, 1);
    }
    
    for (int i = 0; i < m; i++) {
      int x, y; std::cin >> x >> y;
      points.push_back(std::make_pair(x,y));
      
      // common houses are on the right
      sum_x += x;
      sum_y += y;
      
      // ax+by+c >= 0
      lp.set_a(a, n+i, -x);
      lp.set_a(b, n+i, -y);
      lp.set_a(c, n+i, -1);
    }
    
    lp.set_l(a, true, 1); // non-horizontal
    lp.set_l(z, true, 0); // distance constraint
    
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    
    
    if(sol.is_infeasible()) {
      std::cout << "Yuck!\n";
    } else {
      
      //add sum of sewage pipes contraint
      if(s != -1) {
        lp.set_a(a, m + n , sum_x - s);
        lp.set_a(b, m + n , sum_y);
        lp.set_a(c, m + n , m-n);
       
        sol = CGAL::solve_linear_program(lp, ET());
        assert(sol.solves_linear_program(lp));
      
        if(sol.is_infeasible()) {
          std::cout << "Bankrupt!\n";
          T--;
          continue;
        }   
      }
      
      
      for(int i = 1; i <= m + n; i++) {
        int x = points.at(i-1).first;
        int y = points.at(i-1).second;
        
        lp.set_a(b, m + n + i, -x);
        lp.set_a(a, m + n + i, y);
        lp.set_a(d, m + n + i, 1);
        lp.set_a(z, m + n + i, -1);
        
        lp.set_a(b, 2 * (m + n) + i, x);
        lp.set_a(a, 2 * (m + n) + i, -y);
        lp.set_a(d, 2 * (m + n) + i, -1);
        lp.set_a(z, 2 * (m + n ) + i, -1);
      }
      
      lp.set_c(z, 1);
      sol = CGAL::solve_linear_program(lp, ET());
      assert(sol.solves_linear_program(lp));
      std::cout <<  round_up(sol.objective_value()) << std::endl;
    }
    
    T--;
  }
  
  return 0;
}