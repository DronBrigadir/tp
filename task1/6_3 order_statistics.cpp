//Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
//Требуется найти k-ю порядковую статистику, т.е. напечатать число,
//которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
//Реализуйте стратегию выбора опорного элемента “случайный элемент”.
//Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
//
//Требования: к дополнительной памяти: O(n).
//Среднее время работы: O(n). Должна быть отдельно выделенная функция partition.
//Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

#include <iostream>
#include <sstream>
#include <exception>
#include <cstring>
#include <cassert>

void swap(int *array, int pos_first, int pos_second) {
    int tmp = array[pos_second];
    array[pos_second] = array[pos_first];
    array[pos_first] = tmp;
}

template <typename T>
class DefaultComparator
{
public:
    bool operator()(const T &lhs, const T &rhs)
    {
        return lhs < rhs;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T *array, int begin, int end, Comparator cmp = Comparator()) {
    size_t i = begin;

    int sup_elem_pos = rand() % (end - begin + 1) + begin;
    swap(array, sup_elem_pos, end);

    for (int j = begin; j < end; j++) {
        if (cmp(array[j], array[end]) || array[j]== array[end]) {
            swap(array, i, j);
            i++;
        }
    }

    swap(array, i, end);
    return i;
}

template <typename T, typename Comparator = DefaultComparator<T>>
int find_order_statisctics(T *array, int n, int k, Comparator cmp = Comparator()) {
    int begin = 0;
    int end = n - 1;

    bool isFind = false;
    int elem_pos = 0;

    while (!isFind) {
        elem_pos = partition(array, begin, end, cmp);

        if (elem_pos == k) {
            isFind = true;
        } else if (k < elem_pos) {
            end = elem_pos;
        } else {
            begin = elem_pos + 1;
        }
    }

    return elem_pos;
}

void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    size_t k = 0;
    in >> n >> k;

    int *array = new int [n];

    for (size_t i = 0; i < n; i++) {
        in >> array[i];
    }

    int order_statistics_pos = find_order_statisctics(array, n, k);
    int order_statistics = array[order_statistics_pos];
    delete[] array;

    out << order_statistics;
}

void test() {
    {
        std::stringstream input;
        input << "10 4" << std::endl;
        input << "1 2 3 4 5 6 7 8 9 10" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        input << "10 0" << std::endl;
        input << "3 6 5 7 2 9 8 10 4 1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        input << "10 9" << std::endl;
        input << "0 0 0 0 0 0 0 0 0 1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "1");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}