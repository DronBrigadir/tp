// Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
// равного или превосходящего B[i]: A[k] >= B[i].
// Если такого элемента нет, выведите n.
// Время работы поиска k для каждого элемента B[i]: O(log(k)). n, m ≤ 10000.

// В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.

#include <iostream>
#include <cassert>
#include <sstream>

#define DEFAULT 0
#define FIRST_ELEM -1
#define NOT_FOUND -2

class Find {
public:
    Find(int *a, int *b, int n, int m) : a_(a), b_(b), n_(n), m_(m) {}

    ~Find() {
        delete [] a_;
        delete [] b_;
    }

    std::string GetAnswer() {
        return GenAnswer();
    }

private:
    int n_;
    int m_;
    int *a_;
    int *b_;
    int BinSearch(int elem, int left, int right) {
        bool isFind = false;
        int mid = 0;

        while (left <= right && isFind !=true) {
            mid = (left + right) / 2;
            if (a_[mid] >= elem && a_[mid - 1] < elem) {
                isFind = true;
            } else if (a_[mid] > elem) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        assert(isFind == true);
        return mid;
    }

    int ExpSearch(int elem, int &l, int &r) {
        if (a_[0] >= elem) {
            return FIRST_ELEM;
        } else if (n_ == 1) {
            return NOT_FOUND;
        } else {
            int end = n_ == 2 ? 1 : 2;
            while (end <= (n_ - 1)) {
                if (a_[end] >= elem) {
                    l = end / 2;
                    r = end;
                    return DEFAULT;
                }

                end *= 2;
            }

            if (a_[n_ - 1] >= elem) {
                l = end / 2;
                r = n_ - 1;
                return DEFAULT;
            } else {
                return NOT_FOUND;
            }
        }
    }
    std::string GenAnswer() {
        std::stringstream answer;

        for (size_t i = 0; i < m_; i++) {
            int l = 0;
            int r = 0;

            int returnValue = ExpSearch(b_[i], l, r);

            if (returnValue == FIRST_ELEM) {
                answer << "0 ";
            } else if (returnValue == NOT_FOUND) {
                answer << n_ << ' ';
            } else {
                answer << BinSearch(b_[i], l, r) << ' ';
            }
        }

        std::string tmp = answer.str();
        tmp.back() = '\n';
        return tmp;
    }
};

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    int m = 0;
    in >> n >> m;

    int *a = new int[n];
    for (size_t i = 0; i < n; i++) {
        in >> a[i];
    }

    int *b = new int[m];
    for (size_t i = 0; i < m; i++) {
        in >> b[i];
    }

    Find find = Find(a, b, n, m);

    out << find.GetAnswer();
}

void test() {
    {
        std::stringstream input;
        input << "2 1" << std::endl;
        input << "1 2" << std::endl;
        input << "2" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        input << "4 3" << std::endl;
        input << "2 4 5 7" << std::endl;
        input << "4 6 1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "1 3 0\n");
    }
    {
        std::stringstream input;
        input << "9 1" << std::endl;
        input << "1 1 1 1 1 15 15 15 15" << std::endl;
        input << "8" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "5\n");
    }
}

int main () {
    //test();
    run(std::cin, std::cout);

    return 0;
}