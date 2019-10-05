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

#include <list>

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

template<typename T>
bool less(const T& lhs, const T& rhs)
{
    return lhs < rhs;
}

class MyException
{
public:
    MyException(std::string error) : mError(error) {}
    std::string GetError() {
        return mError;
    }

private:
    std::string mError;
};

template <typename T, class Compare>
class Heap {
public:
    Heap(Compare cmp) {
        comparator_ = cmp;
        array_ = Array<T>();
    }
    ~Heap() {}
    void Insert(int elem) {
        array_.Add(elem);
        SiftUp(array_.GetSize() - 1);
    }
    int ExtractRoot() {
        if (array_.GetSize() == 0) {
            throw MyException("Heap is empty");
        }

        int result = array_[0];

        array_[0] = array_[array_.GetSize() - 1];
        array_.DeleteLast();

        if (array_.GetSize() != 0) {
            SiftDown(0);
        }

        return result;
    }
    int PeekRoot() {
        if (array_.GetSize() == 0) {
            throw MyException("Heap is empty");
        }
        return array_[0];
    }
    int GetSize() {
        return array_.GetSize();
    }
private:
    Array<T> array_;
    Compare comparator_;
    void SiftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comparator_(array_[parent], array_[index]) || array_[parent] == array_[index]) {
                return;
            }
            std::swap(array_[index], array_[parent]);
            index = parent;
        }
    }
    void SiftDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int largest = index;

        if (left < array_.GetSize() && comparator_(array_[left], array_[index])) {
            largest = left;
        }
        if (right < array_.GetSize() && comparator_(array_[right], array_[largest])) {
            largest = right;
        }

        if (largest != index) {
            std::swap(array_[index], array_[largest]);
            SiftDown(largest);
        }
    }
};

void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    in >> n;

    Heap<int, std::less<int>> heap = Heap<int, std::less<int>>(std::less<int>());

    for (size_t i = 0; i < n; i++) {
        int arrivalTime = 0;
        int departureTime = 0;
        in >> arrivalTime >> departureTime;

        try {
            int root = heap.PeekRoot();

            if (arrivalTime > root) {
                heap.ExtractRoot();
                heap.Insert(departureTime);
            } else {
                heap.Insert(departureTime);
            }
        }
        catch (MyException &exception) {
            heap.Insert(departureTime);
        }
        catch (std::exception &exception) {
            std::cerr << exception.what() << std::endl;
        }
    }

    int numOfDeadlocks = heap.GetSize();
    out << numOfDeadlocks;
}

void test() {
    {
        std::stringstream input;
        input << "1" << std::endl;
        input << "10 20" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        input << "2" << std::endl;
        input << "10 20" << std::endl;
        input << "20 25" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        input << "3" << std::endl;
        input << "10 20" << std::endl;
        input << "20 25" << std::endl;
        input << "21 30" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        input << "6" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "20 21" << std::endl;
        input << "22 23" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        input << "7" << std::endl;
        input << "10 20" << std::endl;
        input << "15 25" << std::endl;
        input << "21 25" << std::endl;
        input << "25 30" << std::endl;
        input << "25 31" << std::endl;
        input << "31 40" << std::endl;
        input << "31 40" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        input << "6" << std::endl;
        input << "10 20" << std::endl;
        input << "11 20" << std::endl;
        input << "12 20" << std::endl;
        input << "20 30" << std::endl;
        input << "21 31" << std::endl;
        input << "21 40" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        input << "5" << std::endl;
        input << "10 20" << std::endl;
        input << "11 20" << std::endl;
        input << "20 30" << std::endl;
        input << "21 31" << std::endl;
        input << "21 31" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "3");
    }
    {
        std::stringstream input;
        input << "6" << std::endl;
        input << "10 20" << std::endl;
        input << "11 20" << std::endl;
        input << "20 30" << std::endl;
        input << "21 31" << std::endl;
        input << "21 31" << std::endl;
        input << "21 31" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        input << "11" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "10 20" << std::endl;
        input << "21 25" << std::endl;
        input << "21 25" << std::endl;
        input << "21 25" << std::endl;
        input << "21 25" << std::endl;
        input << "21 25" << std::endl;
        input << "26 25" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "5");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}