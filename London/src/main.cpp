///1
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

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
    int h, w; std::cin >> h >> w;
    
    std::string s; std::cin >> s;
    //std::cout << s;
    std::unordered_map<char, int> letter_count_s;
    
    for (const char &c: s) {
        letter_count_s[c]++;   
        //std::cout << c;
    }
    
    int distinct_letters = letter_count_s.size();
    //std::cout << distinct_letters;
  
    graph G(distinct_letters);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    
    std::vector<std::pair<char, char>> text(w*h);
  
    
    for(int i = 0; i < h; i++) {
      for(int j = 0; j < w; j++) {
        char c; std::cin >> c;
        if(letter_count_s[c] != 0)
          text.at(i*w + j).first = c;
        else
          text.at(i*w + j).first = '*';
      }
    }
    
    for(int i = 0; i < h; i++) {
      for(int j = 0; j < w; j++) {
        char c; std::cin >> c;
        if(letter_count_s[c] != 0)
          text.at((i+1)*w - j - 1).second = c;
        else
          text.at((i+1)*w - j - 1).second = '*';
      }
    }
    
    
    int idx = 0;
    std::unordered_map<char, vertex_desc> letter_nodes;
    for(auto it = letter_count_s.cbegin(); it != letter_count_s.cend(); it++) {
      if(it->second != 0) {
        letter_nodes[it->first] = idx;
        idx++;
      
        // add edge from letter to sink with weight = frequency
        adder.add_edge(letter_nodes[it->first], v_sink, it->second);
        //std::cout << it->first << " " << it->second << " ";
      }
    }
    
    //std::cout << std::endl;
    
    std::unordered_map<std::string, int> pair_count_text;
    std::unordered_map<char, int> letter_count_text;
      
    for(int i = 0; i < w*h; i++) {
      char lf = text.at(i).first;
      char lb = text.at(i).second;
      //std::cout << lf << lb << " ";
      
      if(lf == '*' || lb == '*') {
        if(lf != '*') {
          letter_count_text[lf]++;
        } else if (lb != '*') {
          letter_count_text[lb]++;
        }
        
      } else {
        if(lf == lb) {
          letter_count_text[lf]++;
        } else {
          std::string pair;
          pair.push_back(lf);
          pair.push_back(lb);
          pair_count_text[pair]++;
        }
        
      }
        
    }
    
    //std::cout << std::endl;
    
    for(auto it = letter_count_text.cbegin(); it != letter_count_text.cend(); it++) {
      adder.add_edge(v_source, letter_nodes[it->first], it->second);
      //std::cout << it->second << " ";
    }
    
    for(auto it = pair_count_text.cbegin(); it != pair_count_text.cend(); it++) {
      const vertex_desc node = boost::add_vertex(G);
      char lf = (it->first)[0];
      char lb = (it->first)[1];
      adder.add_edge(node, letter_nodes[lf], it->second);
      adder.add_edge(node, letter_nodes[lb], it->second);
      adder.add_edge(v_source, node, it->second);
      //std::cout << lf << lb << " " << it->second << " ";
    }
    
      
    
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    //std::cout << flow;
    
    if(flow == s.size()) {
      std::cout << "Yes\n";
    } else {
      std::cout << "No\n";
    }
    
    
    
    T--;
  }
  
  
  return 0;
}