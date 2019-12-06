#include <cassert>

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(const IGraph &graph) : adjacency_matrix(graph.VerticesCount(),
        std::vector<bool>(graph.VerticesCount(), false))
{
    for (unsigned int from = 0; from < graph.VerticesCount(); from++)
    {
        for (int to : graph.GetNextVertices(from))
        {
            this->AddEdge(from, to);
        }
    }
}

int MatrixGraph::VerticesCount() const
{
    return (int)adjacency_matrix.size();
}

void MatrixGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from < this->VerticesCount());
    assert(to >= 0 && to < this->VerticesCount());

    adjacency_matrix[from][to] = true;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> next_vertices;

    for (unsigned int i = 0; i < adjacency_matrix[vertex].size(); i++)
    {
        if (adjacency_matrix[vertex][i])
        {
            next_vertices.push_back(i);
        }
    }

    return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> prev_vertices;

    for (unsigned int i = 0; i < adjacency_matrix.size(); i++)
    {
        if (adjacency_matrix[i][vertex])
        {
            prev_vertices.push_back(i);
        }
    }

    return prev_vertices;
}
