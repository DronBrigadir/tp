// Binary MSD для long long.
// Дан массив неотрицательных целых 64-разрядных чисел.
// Количество чисел не больше 10^6.
// Отсортировать массив методом MSD по битам (бинарный QuickSort).

#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

template <typename T>
class Array {
public:
    explicit Array (int size) {
        curSize_ = 0;
        size_ = size;
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
    void Print(std::ostream &out) {
        std::stringstream printArray;
        for (size_t i = 0; i < this->GetSize(); i++) {
            printArray << array_[i] << ' ';
        }
        std::string tmp = printArray.str();
        tmp.back() = '\n';
        out << tmp;
    }
    void Swap (int l, int r) {
        T tmp = array_[l];
        array_[l] = array_[r];
        array_[r] = tmp;
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
bool getBit(const T val, unsigned int bitNum) {
    return (val >> bitNum) & 1 == 1;
}

template <typename T>
void helpBinQuickSort(Array<T> *arr, int l, int r, unsigned int bitNumber) {
    if (l == r) {
        return;
    }

    int i = l;
    int j = r;

    while (i != j) {
        while (!getBit((*arr)[i], bitNumber) && i < j) {
            i++;
        }
        while (getBit((*arr)[j], bitNumber) && j > i) {
            j--;
        }

        if (i != j) {
            arr->Swap(i, j);
        }
    }

    if (!getBit((*arr)[r], bitNumber) || getBit((*arr)[l], bitNumber)) {
        helpBinQuickSort(arr, l, r, bitNumber - 1);
        return;
    }

    helpBinQuickSort(arr, l, j - 1, bitNumber - 1);
    helpBinQuickSort(arr, j, r, bitNumber - 1);
}

template <typename T>
void binQuickSort(Array<T> *arr, int l, int r) {
    if (arr->GetSize() < r) {
        throw std::out_of_range("The right border is larger than the array size");
    }

    if (l > r) {
        throw std::invalid_argument("The right border should be larger than the left");
    }

    if (l == r) {
        return;
    }

    unsigned int highBitNumber = sizeof(T) * 8 - 1;
    helpBinQuickSort(arr, l, r, highBitNumber);
}



void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    in >> n;
    assert(n < 1000000);

    Array<unsigned long long> array = Array<unsigned long long>(n);

    for (size_t i = 0; i < n; i++) {
        unsigned long long tmp = 0;
        in >> tmp;

        array.Add(tmp);
    }

    binQuickSort(&array, 0, array.GetSize() - 1);
    array.Print(out);
}

void test() {
    {
        std::stringstream input;
        input << "3" << std::endl;
        input << "4 1000000 7 " << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4 7 1000000\n");
    }
    {
        std::stringstream input;
        input << "5" << std::endl;
        input << "5 3 24 21 19" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "3 5 19 21 24\n");
    }
}

int main () {
    test();
    //run(std::cin, std::cout);
    return 0;
}