#include <iostream>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"
#include "Utils.h"

int main(int argc, const char * argv[]) {
    ListGraph list_graph(11);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 5);
    list_graph.AddEdge(1, 2);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(1, 5);
    list_graph.AddEdge(1, 6);
    list_graph.AddEdge(3, 2);
    list_graph.AddEdge(3, 4);
    list_graph.AddEdge(3, 6);
    list_graph.AddEdge(5, 4);
    list_graph.AddEdge(5, 6);
    list_graph.AddEdge(6, 4);
    list_graph.AddEdge(7, 8);
    list_graph.AddEdge(7, 9);
    list_graph.AddEdge(10, 0);

    std::cout << "list_graph:" << std::endl;
    print(list_graph);

    // Создание ListGraph из другого графа
    ListGraph list_graph_copy(list_graph);
    std::cout << "copy list_graph:" << std::endl;
    print(list_graph_copy);

    // Создание MatrixGraph из другого графа
    MatrixGraph matrix_graph(list_graph);
    std::cout << "matrix graph:" << std::endl;
    print(matrix_graph);

    // Создание ArcGraph из другого графа
    ArcGraph arc_graph(matrix_graph);
    std::cout << "arc graph:" << std::endl;
    print(arc_graph);

    // Создание SetGraph из другого графа
    SetGraph set_graph(arc_graph);
    std::cout << "set graph:" << std::endl;
    print(set_graph);

    return 0;
}