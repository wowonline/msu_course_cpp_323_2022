#include "graph.hpp"
constexpr int kVerticesCount = 14;

int main() {
  auto graph = uni_course_cpp::Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.addVertex();
  }

  graph.addEdge(0, 1);
  graph.addEdge(0, 2);
  graph.addEdge(0, 3);
  graph.addEdge(1, 4);
  graph.addEdge(1, 5);
  graph.addEdge(1, 6);
  graph.addEdge(2, 7);
  graph.addEdge(2, 8);
  graph.addEdge(3, 9);
  graph.addEdge(4, 10);
  graph.addEdge(5, 10);
  graph.addEdge(6, 10);
  graph.addEdge(7, 11);
  graph.addEdge(8, 11);
  graph.addEdge(9, 12);
  graph.addEdge(10, 13);
  graph.addEdge(11, 13);
  graph.addEdge(12, 13);

  return 0;
}
