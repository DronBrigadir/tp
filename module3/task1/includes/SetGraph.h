#ifndef MODULE3_SET_GRAPH_H
#define MODULE3_SET_GRAPH_H

#include <unordered_set>

#include "IGraph.h"

class SetGraph : public IGraph {
public:
    explicit SetGraph(int n) : edges(n), number_of_vertices(n) {}
    explicit SetGraph(const IGraph &graph);
    ~SetGraph() {}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int number_of_vertices;
    std::vector<std::unordered_set<int>> edges;
};


#endif  // MODULE3_SET_GRAPH_H
