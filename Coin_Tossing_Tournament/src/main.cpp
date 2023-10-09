#include <iostream>
#include <vector>
#include <utility>


// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int T; std::cin >> T;
  while(T) {
    int n, m; std::cin >> n >> m;
    
    std::vector<int> scores(n);
    std::vector<int> calc_scores(n);
    std::vector<std::pair<int, int>> matches;
    for(int i = 0; i < m; i++) {
      int a, b, c; std::cin >> a >> b >> c;
      
      if(c == 0) {
        matches.push_back(std::make_pair(a,b));
      } else if (c == 1) {
        calc_scores.at(a)++;
      } else if (c == 2) {
        calc_scores.at(b)++;
      }
    }
    
    for(int i = 0; i < n; i++) {
      int s; std::cin >> s;
      scores.at(i) = s;
    }
    
    int m0 = matches.size();
    //std::cout << m0;
    
    graph G(n + m0);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    for(int i = 0; i < m0; i++) {
      int a = matches.at(i).first;
      int b = matches.at(i).second;
      adder.add_edge(v_source, i, 1);
      adder.add_edge(i, m0 + a, 1);
      adder.add_edge(i, m0 + b, 1);
    }
    
    int sum = 0;
    bool scores_ok = true;
    for(int i = 0; i < n; i++) {
      int dif = scores.at(i) - calc_scores.at(i);
      sum += dif;
      //std::cout << dif << " ";
      if(dif < 0) {
        scores_ok = false;
        break;
      }
      
      adder.add_edge(m0 + i, v_sink, dif);
    }
    
    //std::cout << sum << " ";
    
    if(scores_ok && sum == m0) {
      int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
      //std::cout << flow;
      if(flow == sum) {
        std::cout << "yes\n";
      } else {
        std::cout << "no\n";
      }
      
    } else {
      std::cout << "no\n";
    }
    
    T--;
  }
  
  return 0;
}