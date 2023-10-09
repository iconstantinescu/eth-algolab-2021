#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n; std::cin >> n;
    
    std::vector<int> psum(n);
    int even = 0, odd = 0;
    for(int i = 0; i < n; i++) {
      int a; std::cin >> a;
      psum[i+1] = (psum[i] + a) % 2;
      
      if(psum[i+1] == 1) {
        odd++;
      } else {
        even++;
      }
    }
    
    int count = even * (even-1) / 2 + odd * (odd - 1) / 2 + even;
    
    std::cout << count << std::endl;
    
    T--;
  }
  
  return 0;
}