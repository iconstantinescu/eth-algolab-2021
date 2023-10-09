#include <climits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <CGAL/Gmpq.h>


void testcase() {
  
  int n; std::cin >> n;
  std::vector<std::pair<int64_t, int>> bikers;
  std::vector<CGAL::Gmpq> slopes;
  for(int i = 0; i < n; i++) {
    int64_t y0, x1, y1; std::cin >> y0 >> x1 >> y1;
    bikers.push_back(std::make_pair(y0, i));
    CGAL::Gmpq biker_slope = CGAL::Gmpq(y1 - y0, x1);
    slopes.push_back(biker_slope);
  }

  std::sort(bikers.begin(), bikers.end());
  std::vector<bool> possible(n, true);

  CGAL::Gmpq  min_slope = slopes[bikers.front().second];
  for(int i = 1; i < n; i++) {
    int idx = bikers[i].second;
    if(CGAL::abs(slopes[idx]) < CGAL::abs(min_slope) 
      || (CGAL::abs(slopes[idx]) == CGAL::abs(min_slope) && (slopes[idx] > min_slope))) {
      min_slope = slopes[idx];
    } else if(slopes[idx] < min_slope) {
      possible[idx] = false;
    }
  }

  min_slope = slopes[bikers.back().second];
  for(int i = n-2; i >= 0; i--) {

    int idx = bikers[i].second;
    if(CGAL::abs(slopes[idx]) <= CGAL::abs(min_slope)) {
      min_slope = slopes[idx];
    } else if(slopes[idx] > min_slope) {
      possible[idx] = false;
    }
  }

  for(int i = 0; i < n; i++) {
    if(possible[i])
      std::cout << i << " ";
  }

  std::cout << std::endl;
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
  
}
