#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  
  for(int t = 0; t < T; t++) {
    int n;
    std::cin >> n;
    
    int s = 0;
    for(int i = 0; i < n; i++) {
      int a;
      std::cin >> a;
      s += a;
    }
    
    std::cout << s << std::endl;
  }
  
  
  return 0;
}