///3
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
  
  int n, k, w; std::cin >> n >> k >> w;


  std::vector<int> cost(n);
  for(int i = 0; i < n; i++) {
    int c; std::cin >> c;
    cost[i] = c;
  }

  std::vector<std::vector<int>> waterways(w);
  for(int i = 0; i < w; i++) {
    int l; std::cin >> l;
    for(int j = 0; j < l; j++) {
      int r; std::cin >> r;
      waterways[i].push_back(r);    
    }
  }

  int max_islands = 0;


  for(int i = 0; i < w; i++) {
    int sum = 0;
    int start = 0;
    int end = 0;
    
    while(end < waterways[i].size()) {
      while(end < waterways[i].size() && sum < k) {
        sum += cost[waterways[i][end]];
        end++;
        //std::cout << sum << " ";
      }

      while(start < end && sum > k) {
        sum -= cost[waterways[i][start]];
        //std::cout << sum << " ";
        start++;
      }

      //std::cout << sum << " ";
      if(sum == k) {
        max_islands = std::max(max_islands, end - start);

        // be careful with seg fault
        if(end < waterways[i].size()) {
          sum += cost[waterways[i][end]];
          end++;
        }
      }
      
    }
  }

  std::vector<int> maxi_for_sum(k+1, 0);
  for(int i = 0; i < w; i++) {
    int sum = 0;
    for(int j = 1; j < waterways[i].size(); j++) {
      int r = waterways[i][j];
      sum += cost[r];

      if(sum + cost[0] >= k)
        break;

      if(maxi_for_sum[k - sum - cost[0]] != 0) {
        max_islands = std::max(max_islands, maxi_for_sum[k - sum - cost[0]] + j + 1);
      }
    }

    // so that is does not contain the watewray iself when checking (check i with  i-1 ... 0 only)
    sum = 0;
    for(int j = 1; j < waterways[i].size(); j++) {
      sum += cost[waterways[i][j]];

      if(sum > k)
        break;

      maxi_for_sum[sum] = std::max(maxi_for_sum[sum], j);
    }
  }

  std::cout << max_islands << '\n';

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}


