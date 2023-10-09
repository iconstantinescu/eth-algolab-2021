#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

long solve_dp(std::vector<std::vector<long>> &dp, std::vector<std::map<int, int>> &neighbours, 
        int idx, int k) {

  if(k == 0) {
    return 0;
  }

  if(dp[idx][k] != -1) {
    return dp[idx][k];
  }


  if(neighbours[idx].size() > 0) {
    long best_move = 0;
    for(auto it = neighbours[idx].begin(); it != neighbours[idx].end(); it++) {
      best_move = std::max(best_move, solve_dp(dp, neighbours, (*it).first, k-1) + (*it).second);
    }
    dp[idx][k] = best_move;

  } else {
    dp[idx][k] = solve_dp(dp, neighbours, 0, k);
  }
  
  return dp[idx][k];
}

void testcase() {
  
  int n, m, k;
  long x; 
  std::cin >> n >> m >> x >> k;

  std::vector<std::map<int, int>> neighbours(n);
  for(int i = 0; i < m; i++) {
    int u, v, p; std::cin >> u >> v >> p;

    int curr_points = neighbours[u][v];
    neighbours[u][v] = std::max(curr_points, p);
  }

  //std::cout << neighbours[5].size();

  std::vector<std::vector<long>> dp(n, std::vector<long>(k+1, -1));

  for(int i = 1; i <= k; i++) {
    solve_dp(dp, neighbours, 0, i);
  }

  for(int i = 1; i <= k; i++) {
    if(dp[0][i] >= x) {
      std::cout << i << '\n';
      return;
    }
  }



  std::cout << "Impossible" << '\n';

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
