#include <cassert>
#include <set>

#include "ArcGraph.h"

ArcGraph::ArcGraph(const IGraph &graph)
{
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        vertices.insert(i);
    }

    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        for (int to : graph.GetNextVertices(from))
        {
            this->AddEdge(from, to);
        }
    }
}

void ArcGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from < this->VerticesCount());
    assert(to >= 0 && to < this->VerticesCount());

    pairs_vector.push_back(std::pair<int, int>(from, to));
}

int ArcGraph::VerticesCount() const
{
    return vertices.size();
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> next_vertices;

    for (std::pair<int, int> edge : pairs_vector)
    {
        if (edge.first == vertex)
        {
            next_vertices.push_back(edge.second);
        }
    }

    return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> prev_vertices;

    for (std::pair<int, int> edge : pairs_vector)
    {
        if (edge.second == vertex)
        {
            prev_vertices.push_back(edge.first);
        }
    }

    return prev_vertices;
}

ArcGraph::ArcGraph(int n)
{
    for (int i = 0; i < n; i++)
    {
        vertices.insert(i);
    }
}
