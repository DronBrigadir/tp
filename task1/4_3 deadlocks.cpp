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

struct Node {
    Node(int departureTime) : departureTime(departureTime), next(nullptr) {}
    int departureTime;
    Node *next;
};

class List {
public:
    List() : head_(nullptr), tail_(nullptr), minNode_(nullptr), numOfDeadlocks_(0) {}
    ~List() {
        while (head_ != nullptr) {
            Node *oldHead = head_;
            head_ = head_->next;
            delete oldHead;
        }
    }
    void Insert(int arrivalTime, int departureTime) {
        if (numOfDeadlocks_ == 0) {
            Node *newNode = new Node(departureTime);
            head_ = newNode;
            tail_ = newNode;
            minNode_ = newNode;
            numOfDeadlocks_++;
            return;
        }

        if (arrivalTime > minNode_->departureTime) {
            minNode_->departureTime = departureTime;
            UpdateMin();
        } else {
            numOfDeadlocks_++;
            Node *newNode = new Node(departureTime);
            tail_->next = newNode;
            tail_ = newNode;
            UpdateMin();
        }
    }
    int GetNumOfDeadlocks() {
        return numOfDeadlocks_;
    }

private:
    Node *head_;
    Node *tail_;
    Node *minNode_;
    int numOfDeadlocks_;
    void UpdateMin() {
        if (head_ == tail_) {
            minNode_ = head_;
            return;
        }

        Node *newMin = head_;
        Node *curNode = head_->next;

        do {
            if (newMin->departureTime > curNode->departureTime) {
                newMin = curNode;
            }
            curNode = curNode->next;
        } while (curNode != tail_->next);

        minNode_ = newMin;
    }
};

void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    in >> n;

    List list = List();

    for (size_t i = 0; i < n; i++) {
        int arrivalTime = 0;
        int departureTime = 0;
        in >> arrivalTime >> departureTime;
        list.Insert(arrivalTime, departureTime);
    }

    int numOfDeadlocks = list.GetNumOfDeadlocks();
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
        input << "0" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "0");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}