#include <cassert>

#include "ListGraph.h"

ListGraph::ListGraph(const IGraph &graph) : adjacency_lists(graph.VerticesCount())
{
    for (unsigned int i = 0; i < adjacency_lists.size(); i++)
    {
        adjacency_lists[i] = graph.GetNextVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from < this->VerticesCount());
    assert(to >= 0 && to < this->VerticesCount());

    adjacency_lists[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return (int)adjacency_lists.size();
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < this->VerticesCount());

    std::vector<int> prev_vertices;

    for (unsigned int from = 0; from < adjacency_lists.size(); from++)
    {
        for (int to: adjacency_lists[from])
        {
            if (to == vertex)
            {
                prev_vertices.push_back(from);
            }
        }
    }
    return prev_vertices;
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    assert(0 <= vertex && vertex < this->VerticesCount());
    return adjacency_lists[vertex];
}
