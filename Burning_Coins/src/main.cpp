///3
#include <iostream>
#include <vector>


int solve_dp(std::vector<int> &v, std::vector<std::vector<int>> &dp, int i, int j, bool turn) {
  if(i > j)
    return 0;
  
  if(dp[i][j] != -1)
    return dp[i][j];
    
  if(turn) {
    int val = std::max(solve_dp(v, dp, i+1, j, !turn) + v[i], solve_dp(v, dp, i, j-1, !turn) + v[j]);
    dp[i][j] = val;
  } else {
    int val = std::min(solve_dp(v, dp, i+1, j, !turn), solve_dp(v, dp, i, j-1, !turn));
    dp[i][j] = val;
  }
  
  return dp[i][j];
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n; std::cin >> n;
    std::vector<int> v;
    for(int i = 0; i < n; i++) {
      int a; std::cin >> a;
      v.push_back(a);
    }
    
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));
    solve_dp(v, dp, 0, n-1, true);
    
    std::cout << dp[0][n-1] << std::endl;
    
    T--;
  }
  
  
  return 0;
}