#include <iostream>
#include <vector>
#include <climits>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n, k; std::cin >> n >> k;
    std::vector<int> deck(n);
    std::vector<int> partial_sums(n+1);
    
    for(int i = 0; i < n; i++) {
      int a; std::cin >> a;
      deck[i] = a;
      partial_sums[i+1] = partial_sums[i] + a;
    }
    
    int i = 0, j = 0;
    int best_i = 0, best_j = 0;
    int best_dif = INT_MAX;
    while(j < n) {
      int sum_ij = partial_sums[j+1] - partial_sums[i];
      if(abs(sum_ij - k) < best_dif) {
        best_i = i;
        best_j = j;
        best_dif = abs(sum_ij - k);
      }
      
      if(i == j) {
        j++;
      } else {
        if(sum_ij < k) {
          j++;
        } else if (sum_ij > k) {
          i++;
        } else if (sum_ij == k) {
          break;
        }
      }
    }
    
    std::cout << best_i << " " << best_j << std::endl;
    
    T--;
  }
  
  return 0;
}