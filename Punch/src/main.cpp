#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

// min_cost, max_different, last_take (true/false)
std::tuple<int, int, bool> solve_dp(std::vector<std::pair<int, int>> &beverages, 
                  std::vector<std::vector<std::tuple<int, int, bool>>> &dp,
                  int i, int vol) {
  if(i < 0) {
    return {INT32_MAX, 0, false};
  }

  if(vol <= 0) {
    return {0, 0, false};
  }

  if(std::get<0>(dp[i][vol]) != -1) {
    return dp[i][vol];
  }

  std::tuple<int, int, bool> take = solve_dp(beverages, dp, i, vol - beverages[i].second);
  std::tuple<int, int, bool> skip = solve_dp(beverages, dp, i-1, vol);

  // negative different so that we can use the min function in case of equality
  std::tuple<int, int, bool> take_result = {std::get<0>(take) + beverages[i].first, std::get<1>(take) - !std::get<2>(take), true};
  std::tuple<int, int, bool> skip_result = {std::get<0>(skip), std::get<1>(skip), false};

  dp[i][vol] = std::min(skip_result, take_result);
  
  return dp[i][vol];
}


void testcase() {
  
  int n, k; std::cin >> n >> k;

  std::vector<std::pair<int, int>> beverages(n); // cost, volume
  for(int i = 0; i < n; i++) {
    int c, v; std::cin >> c >> v;
    beverages[i] = {c,v};
  }

  std::tuple<int, int, bool> init = {-1,0,false};
  std::vector<std::vector<std::tuple<int, int, bool>>> dp(n, std::vector<std::tuple<int, int, bool>>(k+1, init));
  auto sol = solve_dp(beverages, dp, n-1, k);

  std::cout << std::get<0>(sol) << " " << -std::get<1>(sol) << "\n";  

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
