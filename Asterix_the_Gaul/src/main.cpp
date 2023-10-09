#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void dfs(std::vector<std::pair<long,long>> &movements, std::vector<long> &best_k,
      int i, int k, long dist, long time) {

  best_k[k] = std::max(best_k[k], dist);

  if(i < 0 || k == movements.size()) {
    return;
  }

  if(movements[i].second > time) {
    dfs(movements, best_k, i-1, k, dist, time);
      return;
  } 

  dfs(movements, best_k, i-1, k, dist, time);
  dfs(movements, best_k, i-1, k+1, dist + movements[i].first , time - movements[i].second);
  return;

}


void testcase() {
  
  int n, m;
  long dist, time; 
  std::cin >> n >> m >> dist >> time;
  
  std::vector<std::pair<long,long>> movements(n);
  std::vector<long> s(m);

  for(int i = 0; i < n; i++)  {
    long di, ti; std::cin >> di >> ti;
    movements[i] = {di,ti};
  }

  for(int i = 0; i < m; i++) {
    long si; std::cin >> si;
    s[i] = si;
  }

  std::vector<long> best_k(n+1);
  dfs(movements, best_k, n-1, 0, 0, time - 1);

  int min_extra = INT32_MAX;
  for(int i = 1; i <= n && best_k[i] > 0; i++) {
    //std::cout << best_k[i] << " ";
    long delta = dist - best_k[i];

    if(delta <= 0) {
      std::cout << 0 << '\n';
      return;
    }

    if(m != 0) {
      int idx = std::lower_bound(s.begin(), s.end(), std::ceil(delta / (double)i)) - s.begin();

      if(s[idx] * (long)i >= delta) {
        min_extra = std::min(min_extra, idx + 1);
      }
    }
    
  }

  if(min_extra == INT32_MAX) {
    std::cout << "Panoramix captured\n";
  } else {
    std::cout << min_extra << '\n';
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
