#include <iostream>
#include <vector>

int dp(int i, int j, std::vector<int>& v, int k, int m,  std::vector<std::vector<int>> &memo) {
  if(i > j)
    return 0;
    
  if(memo[i][j] != -1)
    return memo[i][j];
  
  if(k == 0) {
    memo[i][j] = std::max(v.at(i) + dp(i+1, j, v, m-1, m, memo), v.at(j) + dp(i, j-1, v, m-1, m, memo));
    return memo[i][j];
  } else {
    memo[i][j] = std::min(dp(i+1, j, v, k-1, m,  memo), dp(i, j-1, v, k-1, m, memo));
    return memo[i][j];
  }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;

    for (int i = 0 ; i < t; i++) {

        int n, m, k;
        std::cin >> n;
        std::cin >> m;
        std::cin >> k;

        std::vector<int> v;

        for (int j = 0; j < n; j++) {
            int a;
            std::cin >> a;
            v.push_back(a);
        }
        

        std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
        int best = dp(0, n-1, v, k, m, memo);
        std::cout << best << '\n';

    }

    return 0;
}
