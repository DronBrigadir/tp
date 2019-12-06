#include <iostream>
#include "Utils.h"

void print (const IGraph &graph)
{
    for (int vertex = 0; vertex < graph.VerticesCount(); vertex++)
    {
        for (int i : graph.GetNextVertices(vertex))
        {
            std::cout << "Edge from " << vertex << " to " << i << std::endl;
        }
    }

    std::cout << std::endl;
}