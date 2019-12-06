#ifndef MODULE3_LIST_GRAPH_H
#define MODULE3_LIST_GRAPH_H

#include "IGraph.h"

class ListGraph : public IGraph {
public:
    explicit ListGraph(int n) : adjacency_lists(n) {}
    explicit ListGraph(const IGraph &graph);
    ~ListGraph() {}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacency_lists;
};


#endif  // MODULE3_LIST_GRAPH_H
