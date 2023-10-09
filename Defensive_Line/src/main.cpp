///5
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

bool used_all;

int solve_dp(std::vector<std::vector<int>> &dp, std::vector<int> &v, std::map<int, int> &sum_pos, std::vector<int> &psum, 
        int i, int k, int m) {

  if(i < 0) {
    return 0;
  }

  if(m == 0) {
    used_all = true;
    return 0;
  }
  
  if(dp[i][m] != -1) {
    return dp[i][m];
  }

  if(sum_pos.find(psum[i+1] - k) != sum_pos.end()) {
    int idx = sum_pos.find(psum[i+1] - k)->second - 1;
    dp[i][m] = std::max(solve_dp(dp, v, sum_pos, psum, idx, k, m - 1) + (i - idx), solve_dp(dp, v, sum_pos, psum, i-1, k, m));
  } else {
    dp[i][m] = solve_dp(dp, v, sum_pos, psum, i-1, k, m);
  }

  return dp[i][m];
}


void testcase() {
  
  int n, m, k; std::cin >> n >> m >> k;
  used_all = false;
  
  std::vector<int> v;
  std::vector<int> psum(n+1);
  std::map<int, int> sum_pos;
  sum_pos[0] = 0;

  for(int i = 0; i < n; i++) {
    int a; std::cin >> a;
    v.push_back(a);
    psum[i+1] = psum[i] + a;
    sum_pos[psum[i+1]] = i+1;
  }


  std::vector<std::vector<int>> dp(n, std::vector<int>(m + 1, -1));
  solve_dp(dp, v, sum_pos, psum, n-1, k, m);

  if(!used_all) {
    std::cout << "fail" << std::endl;
  } else {
    std::cout << dp[n-1][m] << std::endl;
  }


  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
