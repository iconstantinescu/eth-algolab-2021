#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

struct boat{
  int length;
  int ring;
  
  boat(int length, int ring) {
    this->length = length;
    this->ring = ring;
  }
};


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n; std::cin >> n;
    std::vector<boat> boats;
    for(int i = 0; i < n; i++) {
      int l, r; std::cin >> l >> r;
      boats.push_back(boat(l, r));
    }
    
    struct {
        bool operator()(boat a, boat b) const { 
          return a.ring < b.ring; 
        }
    } customLess;
    
    std::sort(boats.begin(), boats.end(), customLess);
    
    int count = 0;
    int cur_end = INT_MIN;
    int prev_end = INT_MIN;
    
    for(int i = 0; i < n; i++) {
      int b_length = boats.at(i).length;
      int b_ring = boats.at(i).ring;
      if(b_ring >= cur_end) {
        count++;
        prev_end = cur_end;
        cur_end = std::max(cur_end + b_length, b_ring);
      } else {
        int posible_end = std::max(prev_end + b_length, b_ring);
        if(posible_end < cur_end) {
          cur_end = posible_end;
        }
      }
    }
    
    std::cout << count << '\n';
    
    
    T--;
  }
  
  
  return 0;
}