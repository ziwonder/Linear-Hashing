#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "ADS_set.h"
#include <vector>

int main() {

    ADS_set<double> set1;
  /*  set1.insert_element(2);
    std::cout << set1.size() << std::endl;
    set1.insert_element(4);
    //set1.dump();
    std::cout << set1.size() << std::endl;
    set1.insert_element(7);
    //set1.dump();
    set1.insert_element(6);
    set1.insert_element(8);
    //set1.dump();
    set1.insert_element(14);
    //set1.dump();
    set1.insert_element(12);
    //set1.dump();
    set1.insert_element(19);*/
    set1.dump();
    std::initializer_list<double> vec {3, 1.2, 4.3, 3.2,  2.4,45, 2.1, 0.3, 4, 1, 0.7, 5, 8, 0.1, 3,  2, 8.8};
    ADS_set<double> set2;
    set2.insert(vec.begin(), vec.end());
    set2.dump();
   /*  std::initializer_list<char> ch {'w', 't', 'e','r', 'q', 'd', 's','a', 'x', 'l', '3','2', '4','6', '7', '8'};
       ADS_set<char> set3 {ch};
     set3.dump();
     std::cout << set3.count('q') << std::endl;
     std::cout << set3.count('z') << std::endl;
     std::vector<char> vec1{'=', '-', 'q'};
     set3.insert(vec1.begin(), vec1.end());
     set3.dump();
    set2 = set1;
    std::cout << "--------------------------" << std::endl;
    set1.dump();
    set2.dump();

    ADS_set<char> set4 {set3};
    set4.dump();
    set3.dump();
    //set3.erase('x');
    set3.dump();
    set4.clear();
    set4.dump();
    auto it = set3.begin();
    std::cout << *it << *(++it) << *(++it) << *(++it);
    std::cout << *(++it) << *(++it) << *(++it) << *(++it) << *(++it)  << *(++it);
    ++it;
    std::cout << *(it) << *(++it)  << std::endl;
    auto it2 = set1.begin();
    std::cout << *it2 << std::endl;
    set3.erase('7');
    for (const auto& el : set3) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    std::cout << *(set3.find('t'));
    //std::cout << (set4 == set3) << std::endl; */
/*
    std::vector<int> v;
    for(size_t i{0}; i < 50; ++i) {
        v.push_back(rand() % 250);
    }
    ADS_set<int> set1(v.begin(), v.end());
    set1.dump();

    //auto it = set2.x();
    //
    for (auto it = set2.x(); it != set2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    for (auto it = set2.begin(); it != set2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;*/
    //set2.erase(45);
    //set2.erase(2.4);
    set2.insert(9);
    set2.dump();
    /*  for (auto it = set2.y(); it != set2.end(); ++it) {
          std::cout << *it << " ";
      }

      std::cout << std::endl;
      for (auto it = set2.begin(); it != set2.end(); ++it) {
          std::cout << *it << " ";
      }
      std::cout << std::endl      return 0;
  /*
      set2.insert(50);
      ADS_set<double> set5{set2};
      set5.clear();
      std::cout << set5.y().first << " " << set5.y().second << std::endl;
*/
      set2.swap(set1);
      set2.insert({4,5});
      swap(set1,set2);
      //std::cout << set2.y(1).first << " " << set2.y(1).second << std::endl;

}