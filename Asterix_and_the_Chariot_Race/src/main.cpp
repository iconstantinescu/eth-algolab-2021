#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// picked, covered, uncovered
std::tuple<int, int, int> calculate_cost(std::vector<std::vector<int>> &children, std::vector<int> &cost, int node) {
  
  int picked_sum = 0;
  int covered_sum = 0;
  int uncovered_sum = 0;
  int min_selected_diff = INT32_MAX;
  for(auto it = children[node].begin(); it != children[node].end(); it++) {
    std::tuple<int, int, int> prev_state = calculate_cost(children, cost, *it);

    int picked_val = std::get<0>(prev_state);
    int covered_val = std::get<1>(prev_state);
    int uncovered_val = std::get<2>(prev_state);

    picked_sum += picked_val;
    covered_sum += covered_val;
    uncovered_sum += uncovered_val;
    min_selected_diff = std::min(min_selected_diff, picked_val - covered_val);

  }

  int picked_res = uncovered_sum + cost[node];
  int covered_res = std::min(picked_res, covered_sum + min_selected_diff);
  int uncovered_res = std::min(picked_res, covered_sum);

  return std::make_tuple(picked_res, covered_res, uncovered_res);

}

void testcase() {
  
  int n; std::cin >> n;

  std::vector<std::vector<int>> children(n);
  std::vector<int> cost(n);
  for(int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    children[u].push_back(v);
  }

  for(int i = 0; i < n; i++) {
    int c; std::cin >> c;
    cost[i] = c;
  }

  std::tuple<int, int, int> result = calculate_cost(children, cost, 0);

  
  std::cout << std::get<1>(result) << "\n";
  

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
