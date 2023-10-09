///2
#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n; std::cin >> n;
    
    std::vector<int> locations;
    std::queue<int> unincluded;
    std::queue<int> included;
    
    for(int i = 0; i < n; i++) {
      int a; std::cin >> a;
      locations.push_back(a);
    }
    
    std::sort(locations.begin(), locations.end());
    
   for(int i = 0; i < n; i++) {
      unincluded.push(locations[i]);
    }
    
    int start = locations[0];
    int max_locs = 0;
    int min_dist = INT_MAX;
    std::set<int> results;
    
    while(unincluded.size() > 0) {
      
      int top = unincluded.front();
      while(top <= start + 200 && unincluded.size() > 0) {
        included.push(top);
        unincluded.pop();
        top = unincluded.front();
      }
      
      
      int curr_locs = included.size();
      
      if (curr_locs > max_locs) {
        max_locs = curr_locs;
        results.clear();
        min_dist = INT_MAX;
      }
      
      if (curr_locs == max_locs) {
        int mid = (included.back() + included.front()) / 2;
        int dist = std::max(included.back() - mid, mid - included.front());
        if(dist < min_dist) {
          min_dist = dist;
          results.clear();
        }
        
        if(dist == min_dist) {
          results.insert(mid);
          if(mid - included.front() < included.back() - mid) {
            results.insert(mid+1); // positive
          } else if (mid - included.front() > included.back() - mid) {
            results.insert(mid-1); // negative
          }
        }
        
      }
      
      included.pop();
      
      if(included.size() == 0) {
        included.push(unincluded.front());
        unincluded.pop();
      }
      
      start = included.front();
    }
    
    std::cout << max_locs << " " << min_dist << std::endl;
    
    for(auto it = results.begin(); it != results.end(); it++) {
      std::cout << *it << " ";
    }
    
    std::cout << std::endl;
    
    
    T--;
  }
  
  
  return 0;
}