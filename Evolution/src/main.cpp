#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>


void dfs(std::map<std::string, std::vector<std::string>> &children, std::map<std::string, int> &age,
        std::map<std::string, std::vector<int>> &queries, 
        std::map<std::pair<std::string, int>, std::string> &query_results,
        std::vector<std::pair<std::string, int>> &path,
        std::string node) {


  for(auto it = queries[node].begin(); it != queries[node].end(); it++) {
    auto res = std::lower_bound(path.begin(), path.end(), *it, [](const std::pair<std::string, int> & a, const int b){
              return a.second > b;  //  > not <!
          });

    if(res->second > *it) {
      query_results[{node, *it}] = res->first;
    } else {
      query_results[{node, *it}] = res->first;
    }
  }

  
  for(auto it = children[node].begin(); it != children[node].end(); it++) {
    path.push_back({*it, age[*it]});
    dfs(children, age, queries, query_results, path, *it);
    path.pop_back();
  }


}

void testcase() {
  
  int n, q; std::cin >> n >> q;

  std::map<std::string, int> age;
  int max_age = 0;
  std::string root = "";
  for(int i = 0; i < n; i++) {
    std::string s;
    int a; 
    std::cin >> s >> a;
    age[s] = a; 

    if(max_age < a) {
      max_age = a;
      root = s;
    }
  }

  //std::cout << root;

  std::map<std::string, std::string> parent;
  std::map<std::string, std::vector<std::string>> children;
  for(int i = 0; i < n-1; i++) {
    std::string s, p;
    std::cin >> s >> p;
    parent[s] = p; 
    children[p].push_back(s);
  }

  parent[root] = root;

  std::map<std::string, std::vector<int>> queries;
  std::map<std::pair<std::string, int>, std::string> query_results;
  std::vector<std::pair<std::string, int>> query_order;
  for(int i = 0; i < q; i++) {
    std::string s;
    int b; 
    std::cin >> s >> b;

    queries[s].push_back(b);
    query_order.push_back({s,b});

  }

  std::vector<std::pair<std::string, int>> path;
  path.push_back({root, age[root]}); // add this!
  dfs(children, age, queries, query_results, path, root);

  
  for(int i = 0; i < q; i++) {
  std::cout << query_results[query_order[i]];

    if(i < q-1) {
      std::cout << " ";
    }
  }
    
  
  

  std::cout << '\n';

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
