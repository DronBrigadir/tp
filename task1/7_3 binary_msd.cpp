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

void run(std::istream &in, std::ostream &out) {

}

void test() {
    {
        std::stringstream input;
        input << "3" << std::endl;
        input << "4 1000000 7" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4 7 1000000");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}