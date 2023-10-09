#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;


void testcase() {
  
  int n; std::cin >> n;
  VVI p_mat(n+1, VI(n+1, 0));

  VVI mat(n+1, VI(n+1));
  for(int i = 1; i <= n; i++) {
    for(int j = 1; j <= n; j++) {
      int a; std::cin >> a;
      mat[i][j] = a;
      p_mat[i][j] = p_mat[i][j-1] + p_mat[i-1][j] - p_mat[i-1][j-1] + mat[i][j];
    }
  }


  int total = 0;
  for(int i1 = 1; i1 <= n; i1++) {
    for(int i2 = i1; i2 <= n; i2++) {
      VI s(n+1);
      VI ps(n+1, 0);

      int even = 0;
      int odd = 0;
      for(int j = 1; j <= n; j++) {
        s[j] = p_mat[i2][j] - p_mat[i2][j-1] - p_mat[i1-1][j] + p_mat[i1-1][j-1];
        ps[j] = ps[j-1] + s[j];
        if(ps[j] % 2 == 0) {
          even++;
        } else {
          odd++;
        }
      }

      total += even * (even - 1) / 2;
      total += odd * (odd - 1) / 2;
      total += even;
      
    }
  }


  std::cout << total << "\n";
  


  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
