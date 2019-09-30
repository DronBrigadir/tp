// Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
// равного или превосходящего B[i]: A[k] >= B[i].
// Если такого элемента нет, выведите n.
// Время работы поиска k для каждого элемента B[i]: O(log(k)). n, m ≤ 10000.

// В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.

#include <iostream>
#include <cassert>
#include <sstream>

class Find {
public:
    Find (std::istream &in) {
        int n = 0;
        int m = 0;
        in >> n >> m;
        n_ = n;
        m_ = m;

        int *a = new int [n_];
        a_ = a;
        for (size_t i = 0; i < n_; i++) {
            in >> a_[i];
        }

        int *b = new int [m_];
        b_ = b;
        for (size_t i = 0; i < m_; i++) {
            in >> b_[i];
        }
        ExpSearch();
    }

    ~Find() {
        delete [] a_;
        delete [] b_;
    }

    std::string GetAnswer() {
        return answer_;
    }

private:
    int n_;
    int m_;
    int *a_;
    int *b_;
    std::string answer_;
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

    void ExpSearch() {
        std::stringstream answer;
        for (size_t i = 0; i < m_; i++) {
            if (a_[0] >= b_[i]) {
                answer << 0 << ' ';
            } else if (n_ == 1) {
                answer << n_ << ' ';
            } else {
                int end = 1;
                bool isGetIn = false;
                while (!isGetIn) {
                    if (a_[end] >= b_[i]) {
                        int left = end / 2;
                        int right = end;
                        answer << BinSearch(b_[i], left, right) << ' ';
                        isGetIn = true;
                    }
                    end *= 2;
                    if (end > (n_ - 1) && isGetIn == false) {
                        if (a_[n_ - 1] >= b_[i]) {
                            int left = end / 2;
                            int right = n_ - 1;
                            answer << BinSearch(b_[i], left, right) << ' ';
                            isGetIn = true;
                        } else {
                            answer << n_ << ' ';
                            isGetIn = true;
                        }
                    }
                }
            }
        }

        answer_ = answer.str();
        answer_.back() = '\n';
    }
};

void run(std::istream &in, std::ostream &out) {
    Find tmp = Find(in);
    std::string answer = tmp.GetAnswer();
    out << answer;
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
        input << "5 4" << std::endl;
        input << "1 10 7 3 15" << std::endl;
        input << "13 8 7 1" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4 1 1 0\n");
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
    run(std::cin, std::cout);

    return 0;
}