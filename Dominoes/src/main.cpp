#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  for(int t = 0; t < T; t++) {
    int n;
    std::cin >> n;
    std::vector<int> v(n);
    
    for(int i = 0; i < n; i++) {
      std::cin >> v.at(i);
    }
    
    int impulse = v.at(0);
    int j = 0;
    
    while(j < n) {
      impulse--;
      if(impulse == 0)
        break;
        
      if (v.at(j) > impulse) {
        impulse = v.at(j);
      }
      
      j++;
    }
    
    std::cout << j << std::endl;
  }
  
  return 0;
}