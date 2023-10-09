#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>


int solve_dp(std::vector<std::vector<int>> &mat, std::vector<std::vector<std::vector<int>>> &dp, int x_down, int x_up, int moves) {
  
  int y_down = moves - x_down;
  int y_up = moves - x_up; 

  int n = mat.size();
  
  if(x_down >= n || y_down >= n || x_up >=n || y_up >=n) {
    return 0;
  }

  if(dp[x_down][x_up][moves] != -1) {
    return dp[x_down][x_up][moves];
  }

  int best_move = std::max({
    solve_dp(mat, dp, x_down, x_up, moves+1),
    solve_dp(mat, dp, x_down+1, x_up, moves+1),
    solve_dp(mat, dp, x_down, x_up+1, moves+1),
    solve_dp(mat, dp, x_down+1, x_up+1, moves+1),
  });

  dp[x_down][x_up][moves] = best_move + mat[x_down][y_down];
  if(x_down != x_up) {
    dp[x_down][x_up][moves] +=  mat[x_up][y_up];
  }

  return dp[x_down][x_up][moves];
}



void testcase() {
  
  int n; std::cin >> n;

  std::vector<std::vector<int>> mat(n, std::vector<int>(n));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int c; std::cin >> c;
      mat[i][j] = c;
    }
  }


  int max_moves = 2*(n-1);
  std::vector<std::vector<std::vector<int>>> dp(n, std::vector<std::vector<int>>(n, std::vector<int>(max_moves+1 , -1)));

  int res = solve_dp(mat, dp, 0, 0, 0);

  std::cout << res << '\n';


}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
