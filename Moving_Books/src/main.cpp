#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T){
    
    int n, m; std::cin >> n >> m;
    std::vector<int> strengths(n);
    std::vector<int> weights(m);
    
    for(int i = 0; i < n; i++) {
      std::cin >> strengths.at(i);
    }
    
    for(int i = 0; i < m; i++) {
      std::cin >> weights.at(i);
    }
    
    std::sort(strengths.begin(), strengths.end());
    std::sort(weights.begin(), weights.end());
    
    if(weights.at(m-1) > strengths.at(n-1)) {
      std::cout << "impossible" << '\n';
    } else {
      int boxes_per_person = std::max(1, m / n);
      
      int i = 0;
      int j = 0;
      while(i < n && j < m) {
        int cur_boxes = 0;
        while(cur_boxes < boxes_per_person && j < m && strengths.at(i) >= weights.at(j)) {
          cur_boxes++;
          j++;
        }
        
        i++;
        
        if(i < n) {
          boxes_per_person = std::max(1, (m - j) / (n - i));
        }
      }
      
      std::cout << boxes_per_person * 3 - 1 << '\n';
    }
    
    T--;
  }
  
  return 0;
}