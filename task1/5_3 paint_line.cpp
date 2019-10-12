// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).
// На числовой прямой окрасили N отрезков.
// Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти длину окрашенной части числовой прямой.

#include <iostream>
#include <sstream>
#include <exception>
#include <cstring>
#include <cassert>

struct Line {
    Line(int begin, int end) : begin(begin), end(end) {}
    Line() : begin(0), end(0) {}
    int begin;
    int end;
};

bool operator<(const Line &lhs, const Line &rhs)
{
    if (lhs.begin == rhs.begin)
        return lhs.end < rhs.end;
    return lhs.begin < rhs.begin;
}


std::istream& operator>>(std::istream &in, Line &line)
{
    in >> line.begin >> line.end;
    return in;
}

std::ostream& operator<<(std::ostream &out, const Line &line)
{
    out << "(" << line.begin << ", " << line.end << ")";
    return out;
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

class LineComparator
{
public:
    bool operator()(const Line &lhs, const Line &rhs)
    {
        if (lhs.begin == rhs.begin)
            return lhs.end < rhs.end;
        return lhs.begin < rhs.begin;
    }
};

template <typename T>
class Array {
public:
    Array () {
        curSize_ = 0;
        size_ = 4;
        array_ = new T [size_];
    }
    ~Array() {
        delete [] array_;
    }
    void Add(T elem) {
        if (curSize_ == size_) {
            ExpandArray(size_ * 2);
        }

        array_[curSize_++] = elem;
    }
    int GetSize() {
        return curSize_;
    }
    T& operator[] (const int index)
    {
        return array_[index];
    }
    void DeleteLast() {
        curSize_--;
    }
    void Print() {
        for (size_t i = 0; i < this->GetSize(); i++) {
            std::cout << array_[i] << ' ';
        }

        std::cout << std::endl;
    }

private:
    T *array_;
    int size_;
    int curSize_;
    void ExpandArray(int size) {
        T *newArray = new T [size];

        for (size_t i = 0; i < curSize_; i++) {
            newArray[i] = array_[i];
        }

        delete [] array_;
        array_ = newArray;
        size_ = size;
    }
};

template <typename T>
void arrayCopy(Array<T> *dest, int dBegin, int dEnd,
               Array<T> *src, int sBegin, int sEnd) {
    if (dEnd - dBegin != sEnd - sBegin) {
        throw std::invalid_argument("Wrong copy parametres");
    }

    int i = dBegin;

    for (int j = sBegin; j <= sEnd; j++) {
        (*dest)[i++] = (*src)[j];
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void mergeSort(Array<T> *array, int begin, int end, Comparator cmp = Comparator()) {
    if (array->GetSize() < end) {
        throw std::out_of_range("The right border is larger than the array size");
    }

    if (begin > end) {
        throw std::invalid_argument("The right border should be larger than the left");
    }

    if (begin == end) {
        return;
    }

    int length = end - begin + 1;

    int leftLength = length / 2;

    mergeSort(array, begin, begin + leftLength - 1, cmp);
    mergeSort(array, begin + leftLength, end, cmp);

    Array<T> tmp = Array<T>();
    Merge(array, begin, begin + leftLength - 1, begin + leftLength, end, &tmp, cmp);
    arrayCopy(array, begin, end, &tmp, 0, tmp.GetSize() - 1);
}

template <typename T, typename Comparator = DefaultComparator<T>>
void Merge(Array<T> *array, int lBegin, int lEnd, int rBegin, int rEnd,
           Array<T> *dest, Comparator cmp = Comparator()) {
    int lPointer = lBegin;
    int rPointer = rBegin;

    while (lPointer <= lEnd && rPointer <= rEnd) {
        if (cmp((*array)[lPointer], (*array)[rPointer])) {
            dest->Add((*array)[lPointer++]);
        } else {
            dest->Add((*array)[rPointer++]);
        }
    }

    if (lPointer == lEnd + 1) {
        while (rPointer <= rEnd) {
            dest->Add((*array)[rPointer++]);
        }
    } else {
        while (lPointer <= lEnd) {
            dest->Add((*array)[lPointer++]);
        }
    }
}

template <typename T>   
int lengthOfPaintingPart(Array<T> *arr) {
    if (arr->GetSize() == 0) {
        return 0;
    }

    Line prev = (*arr)[0];
    int length = prev.end - prev.begin;
    for (size_t i = 1; i < arr->GetSize(); i++) {
        Line curr = (*arr)[i];

        if (curr.begin >= prev.begin && curr.end <= prev.end) {
            continue;
        } else if (curr.begin <= prev.end && curr.end > prev.end) {
            length += curr.end - prev.end;
            prev = curr;
        } else if (curr.begin > prev.end) {
            length += curr.end - curr.begin;
            prev = curr;
        }
    }

    return length;
}

void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    in >> n;

    Array<Line> array = Array<Line>();

    for (size_t i = 0; i < n; i++) {
        Line tmp;
        in >> tmp;

        array.Add(tmp);
    }

    mergeSort(&array, 0, array.GetSize() - 1, LineComparator());

    int answer = 0;
    answer = lengthOfPaintingPart(&array);

    out << answer;
}

void test() {
    {
        std::stringstream input;
        input << "3" << std::endl;
        input << "1 4" << std::endl;
        input << "7 8" << std::endl;
        input << "2 5" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        input << "4" << std::endl;
        input << "1 4" << std::endl;
        input << "2 4" << std::endl;
        input << "1 8" << std::endl;
        input << "3 5" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "7");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}