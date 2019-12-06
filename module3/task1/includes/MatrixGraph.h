#ifndef MODULE3_MATRIX_GRAPH_H
#define MODULE3_MATRIX_GRAPH_H

#include "IGraph.h"

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(int n) : adjacency_matrix(n, std::vector<bool>(n, false)) {}
    explicit MatrixGraph(const IGraph &graph);
    ~MatrixGraph() {}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<bool>> adjacency_matrix;
};


#endif  // MODULE3_MATRIX_GRAPH_H
