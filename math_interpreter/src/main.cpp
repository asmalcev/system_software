#include <iostream>

#include "./LinkedList/LinkedList.hpp"

int main() {
  LinkedList<int> l;
  l.pushNode(4);
  l.pushNode(3);
  l.pushNode(0);
  l.pushNode(-5);
  l.pushNode(9);

  for (auto it = l.begin(); it != l.end(); ++it) {
    std::cout << it() << std::endl;
  }
}