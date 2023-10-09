#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

long solve_dp(std::vector<std::pair<int,int>> &potions_A, 
        std::vector<std::vector<std::vector<long>>> &dp,
        int i, int k, int h) {

  if(k == 0) {
    if(h == 0) return 0;

    return -1; // H could not be reached
  }

  if(i + 1 < k) {
    return -1; // cannot skip aymore
  }

  if(dp[i][k][h] != -2) {
    return dp[i][k][h];
  }

  long skip = solve_dp(potions_A, dp, i-1, k, h);
  long take = solve_dp(potions_A, dp, i-1, k-1, std::max(0, h - potions_A[i].second));
  
  if(take > -1) {
    dp[i][k][h] = std::max(skip, take + potions_A[i].first);
  } else {
    dp[i][k][h] = skip; // if we skip maybe we can reach H in k 
  }
  
  return dp[i][k][h];
}

void testcase() {
  
  int n,m; std::cin >> n >> m;
  int a,b; std::cin >> a >> b;
  long P, H, W; std::cin >> P >> H >> W;

  std::vector<std::pair<int,int>> potions_A;
  for(int i = 0; i < n; i++) {
    int p,h; std::cin >> p >> h;
    potions_A.push_back({p,h});
  }

  
  std::vector<int> potions_B;
  for(int i = 0; i < m; i++) {
    int w; std::cin >> w;
    potions_B.push_back(w);
  }

  std::sort(potions_B.begin(), potions_B.end(), std::greater<int>());


  std::vector<int> used_b(n+1); // how much pot_b we need based on how many pot_a we use
  
  for(int i = 1; i <= n; i++) {
    long sum_w = 0;
    int count = 0;
    for(int w : potions_B) {
      sum_w += w;
      count++;

      if(sum_w - (long)a*i >= W) { // USE LONG!!
        break;
      }
    }

    if(sum_w - a*i < W)
      used_b[i] = -1; // cannot reach that sum if we use this many pot_A
    else {
      used_b[i] = count;
    }
  }

  std::vector<std::vector<std::vector<long>>> dp(n, std::vector<std::vector<long>>(n+1, std::vector<long>(H+1, -2)));
  
  int used_a = 0;
  for(int k = 1; k <= n; k++) {

    if(used_b[k] == -1) {
      break; // cannot satify W if we reach this k
    }

    solve_dp(potions_A, dp, n-1, k, H);
    if(dp[n-1][k][H] - (long)used_b[k]*b >= P) {  // USE LONG!!
      used_a = k;
      break;
    }
  }

  // could not find enough power, happiness or wit
  if(used_a == 0) {
    std::cout << -1 << '\n';
    return;
  }
  
  std::cout << used_a + used_b[used_a] << '\n';
  
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}