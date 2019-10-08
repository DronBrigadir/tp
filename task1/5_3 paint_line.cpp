// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).
// На числовой прямой окрасили N отрезков.
// Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти длину окрашенной части числовой прямой.

// На вокзале есть некоторое количество тупиков, куда прибывают электрички.
// Этот вокзал является их конечной станцией. Дано расписание движения электричек,
// в котором для каждой электрички указано время ее прибытия, а также время отправления в следующий рейс.
// Электрички в расписании упорядочены по времени прибытия.
// Когда электричка прибывает, ее ставят в свободный тупик с минимальным номером.
// При этом если электричка из какого-то тупика отправилась в момент времени X, то электричку,
// которая прибывает в момент времени X, в этот тупик ставить нельзя, а электричку,
// прибывающую в момент X+1 — можно.
// В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
// Напишите программу, которая по данному расписанию определяет,
// какое минимальное количество тупиков требуется для работы вокзала.

// Вначале вводится n - количество электричек в расписании.
// Затем вводится n строк для каждой электрички, в строке - время прибытия и время отправления.
// Время - натуральное число от 0 до 1 000 000 000. Строки в расписании упорядочены по времени прибытия.

#include <iostream>
#include <cassert>
#include <sstream>
#include <exception>
#include <cstring>

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
    template <typename Comparator = DefaultComparator<T>>
    void mergeSort(int l, int r, Comparator cmp = Comparator()) {
        if (this->GetSize() < r) {
            throw std::out_of_range("The right border is larger than the array size");
        }

        if (l >= r) {
            throw std::invalid_argument("The right border should be larger than the left");
        }

        int length = r - l + 1;
        int leftLength = length / 2;
        int rightLength = length - leftLength;

        try {
            helpMergeSort(&array_, leftLength);
            T *tmpArray = array_ + leftLength;
            helpMergeSort(&tmpArray, rightLength);

            T *tmp = new T [length];
            Merge(array_, leftLength, array_ + leftLength, rightLength, tmp);
            std::memcpy(array_, tmp, sizeof(T) * length);
            delete[] tmp;
        }
        catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
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
    void helpMergeSort(T **array, int length) {
        if (length <= 1) {
            return;
        }

        int leftLength = length / 2;
        int rightLength = length - leftLength;

        helpMergeSort(array, leftLength);
        T *tmpArray = (*array) + leftLength;
        helpMergeSort(&tmpArray, rightLength);

        T *tmp = new T [length];
        Merge(*array, leftLength, (*array) + leftLength, rightLength, tmp);
        std::memcpy(*array, tmp, sizeof(T) * length);
        delete[] tmp;
    }
    template <typename Comparator = DefaultComparator<T>>
    void Merge(T *lArray, int lLength, T *rArray, int rLength, T *dest, Comparator cmp = Comparator()) {
        int lPointer = 0;
        int rPointer = 0;
        int i = 0;

        while (i < (lLength + rLength) && lPointer < lLength && rPointer < rLength) {
            if (cmp(lArray[lPointer], rArray[rPointer])) {
                dest[i] = lArray[lPointer++];
            } else {
                dest[i] = rArray[rPointer++];
            }
            i++;
        }

        if (lPointer == lLength) {
            while (i < (lLength + rLength)) {
                dest[i] = rArray[rPointer++];
                i++;
            }
        } else {
            while (i < (lLength + rLength)) {
                dest[i] = lArray[lPointer++];
                i++;
            }
        }
    }
};

template <typename T>   
int lengthOfPaintingPart(Array<T> arr) {  // TODO() : написать реализацию функции

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

    array.Print();
    array.mergeSort(0, array.GetSize() - 1, LineComparator());
    array.Print();

    int answer = 0;
    answer = lengthOfPaintingPart(array);

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
        //assert(output.str() == "5");
    }
}

int main () {
    test();
    //run(std::cin, std::cout);
    return 0;
}