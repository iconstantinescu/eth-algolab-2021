#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

bool is_inside_triangle(std::vector<P> &triangle, P p) {

  P q0 = triangle[0];
  P q1 = triangle[1];
  P q2 = triangle[2];
  P q3 = triangle[3]; 
  P q4 = triangle[4];
  P q5 = triangle[5]; 

  return !CGAL::right_turn(q0, q1,p) && 
    !CGAL::right_turn(q2,q3,p) && 
    !CGAL::right_turn(q4,q5,p);
  return true;
}

void testcase() {
  
  int m, n; std::cin >> m >> n;

  std::vector<std::pair<P,P>> paths; 

  int x1,y1; std::cin >> x1 >> y1;
  for(int i = 1; i < m; i++) {
    int x2, y2; std::cin >> x2 >> y2;
    paths.push_back({P(x1,y1), P(x2,y2)});
    x1 = x2;
    y1 = y2;
  }


  std::vector<std::vector<P>> triangles(n);
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < 6; j++) {
      int qx, qy; std::cin >> qx >> qy;
      triangles[i].push_back(P(qx, qy));
    }


    //ensure correct (ccw) order for orientation tests!
    for (int j = 0; j < 6; j = j + 2) {
      if (CGAL::right_turn(triangles[i][j], triangles[i][j+1], triangles[i][(j+2) % 6])) {
        std::swap(triangles[i][j], triangles[i][j+1]);
      }  
    }
      
  }

  int b = 0;
  int e = 0;

  std::set<int> in_triangle;
  std::set<int> unincluded;
  std::map<int, int> included_count;

  for(int i = 0; i < m-1; i++) {
    unincluded.insert(i);
  }

  int min_length = n;
  while(e < n) {
    while(e < n && !unincluded.empty()) {
      for(int i = 0; i < m - 1; i++) {
        if(is_inside_triangle(triangles[e], paths[i].first) && is_inside_triangle(triangles[e], paths[i].second)) {
          included_count[i]++;

          if(included_count[i] == 1) {
            unincluded.erase(i);
          }
        }
      }

      e++;
    }

    while(b < e && unincluded.empty()) {
      for(int i = 0; i < m - 1; i++) {
        if(is_inside_triangle(triangles[b], paths[i].first) && is_inside_triangle(triangles[b], paths[i].second)) {
          included_count[i]--;
          if(included_count[i] == 0) {
            unincluded.insert(i);
          }
        }
      }

      b++;
    }

    if(min_length > e - b + 1) {
      min_length = e - b + 1;
    }
  }

  std::cout << min_length << "\n";

  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
