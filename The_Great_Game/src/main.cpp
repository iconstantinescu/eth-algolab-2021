#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int dp(std::vector<int> &mini, std::vector<int> &maxi, 
    std::vector<std::vector<int>> &neighbours,
    int pos, bool turn) {

  if(!turn) {
    if(maxi[pos] != -1)
      return maxi[pos];

    int worst_move = 0;
    for(int next_pos : neighbours[pos]) {
      worst_move = std::max(worst_move, dp(mini, maxi, neighbours, next_pos, !turn));
    }  

    maxi[pos] = worst_move + 1;
    return maxi[pos];

  } else {
    if(mini[pos] != -1)
      return mini[pos];

    int best_move = INT32_MAX;
    for(int next_pos : neighbours[pos]) {
      best_move = std::min(best_move, dp(mini, maxi, neighbours, next_pos, !turn));
    }  

    mini[pos] = best_move + 1;
    return mini[pos];
  }
}

void testcase() {

  
  int n, m; std::cin >> n >> m;
  int r, b; std::cin >> r >> b;

  std::vector<std::vector<int>> neighbours(n+1);
  for(int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    neighbours[u].push_back(v);
  }

  std::vector<int> r_sherlock(n+1, -1);
  std::vector<int> r_moriarty(n+1, -1);
  std::vector<int> b_sherlock(n+1, -1);
  std::vector<int> b_moriarty(n+1, -1);

  r_sherlock[n] = 0;
  r_moriarty[n] = 0;
  b_sherlock[n] = 0;
  b_moriarty[n] = 0;

  dp(r_sherlock, r_moriarty, neighbours, r, true);
  dp(b_moriarty, b_sherlock, neighbours, b, true);


  if(r_sherlock[r] < b_moriarty[b]) {
    std::cout << 0 << "\n";
  } else if (r_sherlock[r] == b_moriarty[b]) {
    std::cout << 1 - r_sherlock[r] % 2 << "\n";
  } else {
    std::cout << 1 << "\n";
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
