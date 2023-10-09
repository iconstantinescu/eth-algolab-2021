#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

struct Chamber{
  int idx;
  int length;
  int galleons;
  int time;
  int count_n;

  Chamber(int idx, int length, int galleons, int time, int count_n) {
    this->idx = idx;
    this->length = length;
    this->galleons = galleons;
    this->time = time;
    this->count_n = count_n;
  }
};

void dfs(std::vector<std::vector<Chamber>> &children, Chamber &curr) {
  
  for(int j = 0; j < children[curr.idx].size(); j++) {
    Chamber &next = children[curr.idx][j];
    dfs(children, next);
    curr.time += next.time + next.length;
    curr.count_n += next.count_n + 1;
  }

}

long maximize_gold(std::vector<std::vector<Chamber>> &children, 
            Chamber &curr, int t) {


  if(children[curr.idx].size() == 0) {
    return curr.galleons - t;
  }
  

  std::sort(children[curr.idx].begin(), children[curr.idx].end(), 
        [](const Chamber &a, const Chamber &b) -> bool {
          const long c1 = a.time + a.length;  // USE LONG!!!!
          const long n1 = a.count_n + 1;
          const long c2 = b.time + b.length;
          const long n2 = b.count_n + 1; 
             return c1*n2 < c2*n1; 
        });

  long gold_sum = 0;
  int delta = t;
  for(int j = 0; j < children[curr.idx].size(); j++) {
    Chamber &next = children[curr.idx][j];
    gold_sum += maximize_gold(children, next, delta + next.length);
    delta += 2 * (next.time + next.length); // back and forth
  }

  return gold_sum + curr.galleons - t;
}

void testcase() {
  

  int n; std::cin >> n;

  std::vector<int> galleons(n+1);
  for(int i = 1; i <= n; i++) {
    int g; std::cin >> g;
    galleons[i] = g;
  }

  std::vector<std::vector<Chamber>> children(n+1); 
  for(int i = 0; i < n; i++) {
    int u, v, l; std::cin >> u >> v >> l;
    children[u].push_back(Chamber(v,l,galleons[v],0,0));
  }

  Chamber root = Chamber(0,0,0,0,0);
  dfs(children, root);

  long max_gold = maximize_gold(children, root, 0);

  // for(int i = 0; i <= n; i++) {
  //   for(int j = 0; j < children[i].size(); j++) {
  //     std::cout << children[i][j].time << " ";
  //   }

  //   std::cout << '\n';
  // }

  std::cout << max_gold << '\n';



  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
