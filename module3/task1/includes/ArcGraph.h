#ifndef MODULE3_ARC_GRAPH_H
#define MODULE3_ARC_GRAPH_H

#include <set>

#include "IGraph.h"

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int n);
    explicit ArcGraph(const IGraph &graph);
    ~ArcGraph() {}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::set<int> vertices;
    std::vector<std::pair<int, int>> pairs_vector;
};


#endif  // MODULE3_ARC_GRAPH_H
