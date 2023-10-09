#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <list>

int n, k, m; 

int unique(std::list<int> &gate) {
  int unique = 0;
  std::vector<bool> already_seen(k, false);
  for(int el :  gate) {
    if (!already_seen[el]) {
          already_seen[el] = true;
          unique++;
       }
  }

  return unique;
}

int gate_state(std::list<int> &gate) {
  int state = 0;
  int base = 1;
  for (auto it = gate.begin(); it != gate.end(); it++) {
    int fighter = *it;

    if (it != gate.begin()) {
      state += fighter * base;
      base *= k;
    }
  }

    return state;
}


// f, state_n , satte_s, imbalance
typedef std::vector<std::vector<std::vector<std::vector<int>>>> dp_table;

int max_excitement(const std::vector<int> &fighters, dp_table &memo,
          std::list<int> north_gate, int p,
          std::list<int> south_gate, int q,
          bool north, int f) {

  if(f == n) {
    return 0;
  }

  if(north) {
    p++;
    north_gate.push_back(fighters[f]);
    if(north_gate.size() > unsigned(m)) {
      north_gate.pop_front();
    }
  } else {
    q++;
    south_gate.push_back(fighters[f]);
    if(south_gate.size() > unsigned(m)) {
      south_gate.pop_front();
    }
  }

  int imbalance = std::abs(p - q); 

  int round_val = 0;
  if(north) {
    round_val = 1000 * unique(north_gate) - (1 << imbalance); 
  } else {
    round_val = 1000 * unique(south_gate) - (1 << imbalance); 
  }

  if(round_val < 0) {
    return std::numeric_limits<int>::min();
  }

  int north_state = gate_state(north_gate);
  int south_state = gate_state(south_gate);


  if(p <= q) {
    std::swap(north_state, south_state);
  }


  if(memo[f+1][north_state][south_state][imbalance] != -1) {
    return round_val + memo[f+1][north_state][south_state][imbalance];
  }
  


  int next_south = max_excitement(fighters, memo, 
                  north_gate, p, south_gate, q,
                  false, f+1);

  int next_north = max_excitement(fighters, memo, 
                  north_gate, p, south_gate, q,
                  true, f+1);

  int excitement = std::max(next_north, next_south);
  if(p >= m && q >= m) {
    memo[f+1][north_state][south_state][imbalance] = excitement;
  }
  
  return round_val + excitement;

}

void testcase() {
  
  std::cin >> n >> k >> m;
  std::vector<int> fighters;
  for(int i = 0; i < n; i++) {
    int a; std::cin >> a;
    fighters.push_back(a);
  }

  int num_states = std::pow(k, m-1);
  dp_table memo(n+1, std::vector<std::vector<std::vector<int>>>(num_states,
          std::vector<std::vector<int>>(num_states,
          std::vector<int>(12, -1))));

  int res = max_excitement(fighters, memo, {}, 0, {}, 0, false, 0);
  std::cout << res << '\n';
  

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}