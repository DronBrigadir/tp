// Реализовать очередь с динамическим зацикленным буфером.
// Обрабатывать команды push back и pop front.
// В первой строке количество команд n. n ≤ 1000000.
//
// Каждая команда задаётся как 2 целых числа: a b.
//
// a = 2 - pop front
// a = 3 - push back
//
// Если дана команда pop front, то число b - ожидаемое значение.
// Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.

#include <iostream>
#include <cassert>
#include <sstream>

class Queue {
public:
    Queue() {
        head_ = 0;
        tail_ = 0;
        curSize_ = 0;
        bufferSize_ = 4;
        data_ = new int [bufferSize_];
    }
    ~Queue() {
        delete [] data_;
    }
    bool IsEmpty() const {
        return curSize_ == 0;
    }
    void Enqueue(int value) {
        if (curSize_ == bufferSize_) {
            ExpandBuffer(bufferSize_ * 2);
        }
        if (IsEmpty()) {
            data_[head_] = value;
        } else {
            tail_++;
            if (tail_ == bufferSize_) {
                tail_ %= bufferSize_;
            }
            data_[tail_] = value;
        }
        curSize_++;
    }
    int Dequeue() {
        if (IsEmpty()) {
            return -1;
        }

        int value = data_[head_];
        curSize_--;
        if (IsEmpty()) {
            return value;
        } else {
            head_++;
            if (head_ == bufferSize_) {
                head_ %= bufferSize_;
            }
        }

        return value;
    }
private:
    int *data_;
    int bufferSize_;
    int head_;
    int tail_;
    int curSize_;
    void ExpandBuffer(int size) {
        int *newData = new int [size];

        int curNew = 0;
        int curOld = head_;
        while (curOld != tail_) {
            newData[curNew++] = data_[curOld++];
            if (curOld == bufferSize_) {
                curOld %= bufferSize_;
            }
        }
        newData[curNew] = data_[tail_];

        delete [] data_;
        data_ = newData;
        head_ = 0;
        tail_ = bufferSize_ - 1;
        bufferSize_ = size;
    }
};

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    in >> n;

    Queue queue = Queue();

    for (size_t i = 0; i < n; i++) {
        int op = 0;
        int val = 0;
        in >> op >> val;

        switch (op)
        {
            case 2: {
                int qVal = queue.Dequeue();
                if (qVal != val) {
                    out << "NO";
                    return;
                }
                break;
            }
            case 3: {
                queue.Enqueue(val);
                break;
            }
            default:
                assert("Wrong operations");
        }
    }

    out << "YES";
}

void testLogic() {
    Queue queue;
    int sampleSize = 10;

    assert(queue.IsEmpty());

    for (int i = 0; i < sampleSize; ++i)
        queue.Enqueue(i);

    assert(!queue.IsEmpty());

    for (int i = 0; i < sampleSize; ++i)
    {
        assert(i == queue.Dequeue());
    }

    assert(queue.IsEmpty());
}

void test() {
    {
        std::stringstream input;
        input << "3" << std::endl;
        input << "3 44" << std::endl;
        input << "3 50" << std::endl;
        input << "2 44" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        input << "2" << std::endl;
        input << "2 -1" << std::endl;
        input << "3 10" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        input << "2" << std::endl;
        input << "3 44" << std::endl;
        input << "2 66" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "NO");
    }
    {
        std::stringstream input;
        input << "11" << std::endl;
        input << "3 1" << std::endl;
        input << "3 2" << std::endl;
        input << "3 3" << std::endl;
        input << "3 4" << std::endl;
        input << "2 1" << std::endl;
        input << "2 2" << std::endl;
        input << "3 1" << std::endl;
        input << "2 3" << std::endl;
        input << "2 4" << std::endl;
        input << "2 1" << std::endl;
        input << "2 -1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        input << "13" << std::endl;
        input << "3 1" << std::endl;
        input << "3 2" << std::endl;
        input << "3 3" << std::endl;
        input << "3 4" << std::endl;
        input << "2 1" << std::endl;
        input << "3 5" << std::endl;
        input << "3 6" << std::endl;
        input << "2 2" << std::endl;
        input << "2 3" << std::endl;
        input << "2 4" << std::endl;
        input << "2 5" << std::endl;
        input << "2 6" << std::endl;
        input << "2 -1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "YES");
    }
}

int main () {
    //test();
    //testLogic();
    run(std::cin, std::cout);
    return 0;
}