// В круг выстроено N человек, пронумерованных числами от 1 до N.
// Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек.
// Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й, затем 2-й,
// затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й. Таким образом, уцелеет 4-й.
//
// Необходимо определить номер уцелевшего.
//
// N, k ≤ 10000.

#include <iostream>
#include <cassert>
#include <sstream>

class FlaviusCounting {
public:
    FlaviusCounting(size_t n, size_t k) {
        n_ = n;
        k_ = k;
        victim_ = -1;
        killer_ = 0;
        squad_ = new bool [n_];
        for (size_t i = 0; i < n_; i++) {
            squad_[i] = true;
        }
    }

    ~FlaviusCounting() {
        delete [] squad_;
    }

    int whoSurvive() {
        if (n_ == 1) {
            return 1;
        }

        size_t deathNumber = 0;
        while (deathNumber != n_ - 1) {
            Iteration();
            deathNumber++;
        }
        return killer_ + 1;
    }

private:
    size_t n_;
    size_t k_;
    size_t killer_;
    size_t victim_;
    bool *squad_;
    void Iteration() {
        for (size_t i = 0; i < k_; i++) {
            do {
                victim_++;
                if (victim_ == n_) {
                    victim_ %= n_;
                }
                if (i == k_ - 2) {
                    killer_ = victim_;
                }
            } while(squad_[victim_] != true);
        }
        squad_[victim_] = false;
    }
};

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    in >> n;
    int k = 0;
    in >> k;

    if (n == 1) {
        out << 1;
    }

    FlaviusCounting tmp = FlaviusCounting(n, k);
    out << FlaviusCounting(n, k).whoSurvive();
}

void test() {
    {
        std::stringstream input;
        input << "10 3" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        input << "7 4" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        input << "12 2" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "9");
    }
    {
        std::stringstream input;
        input << "41 2" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "19");
    }
    {
        std::stringstream input;
        input << "10 9" << std::endl;

        std::stringstream output;

        run(input, output);
        assert(output.str() == "7");
    }
}

int main () {
    run(std::cin, std::cout);
    return 0;
}