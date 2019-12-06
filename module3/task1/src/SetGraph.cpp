#include <cassert>

#include "SetGraph.h"

SetGraph::SetGraph(const IGraph &graph) : edges(graph.VerticesCount()), number_of_vertices(graph.VerticesCount())
{
    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        for (int to : graph.GetNextVertices(from))
        {
            this->AddEdge(from, to);
        }
    }
}

void SetGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from < this->VerticesCount());
    assert(to >= 0 && to < this->VerticesCount());

    edges[from].insert(to);
}

int SetGraph::VerticesCount() const
{
    return number_of_vertices;
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    return std::vector<int>(edges[vertex].begin(), edges[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> prev_vertices;

    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i].find(vertex) != edges[i].end())
        {
            prev_vertices.push_back(i);
        }
    }

    return prev_vertices;
}
