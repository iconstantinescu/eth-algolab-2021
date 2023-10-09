#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <math.h> 

typedef std::pair<int, int> pi;

int deactivate_bombs(int start, int n, std::vector<bool>& deact) {
  if(start < n) {
    if(deact.at(start) ==  true) {
      return deactivate_bombs(2*start+1, n, deact) + deactivate_bombs(2*start+2, n, deact);
    } else {
      deact.at(start) = true;
      return deactivate_bombs(2*start+1, n, deact) + deactivate_bombs(2*start+2, n, deact) + 1;
    }
    
  }
  
  return 0;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  for(int t = 0; t < T; t++) {
    int n;
    std::cin >> n;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> pq;
    
    for(int i = 0; i < n; i++) {
      int a;
      std::cin >> a;
      pq.push(std::make_pair(a,i));
    }
    
    std::vector<bool> deact(n);
    int global_t = 0;
    
    while(!pq.empty()) {
      int btime = pq.top().first;
      int pos = pq.top().second;
      pq.pop();
      
      if(!deact.at(pos)) {
         if(btime <= global_t) {
            std::cout << "no" << '\n';
            break;
          }
          
          int deact_time = deactivate_bombs(pos, n, deact);
          
          if(deact_time + global_t > btime) {
             std::cout << "no" << '\n';
             break;
          }
          
          global_t += deact_time;
        
      } 
    }
    
    if(pq.empty()) {
      std::cout << "yes" << '\n';
    }
    
  }
  
  return 0;
}