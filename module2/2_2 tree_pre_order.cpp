// Дано число N < 10^6 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root->Key <= K,
// то узел K добавляется в правое поддерево root;
// Иначе в левое поддерево root.
// Выведите элементы в порядке pre-order (сверху вниз).
// Рекурсия запрещена.

#include <iostream>
#include <sstream>
#include <cassert>
#include <queue>
#include <stack>

template <typename T>
struct Node {
    T value;
    Node *left;
    Node *right;

    explicit Node(T value) : value(value), left(nullptr), right(nullptr) {}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Node<T>& obj) {
    os << obj.value;
    return os;
}

template <typename T>
class BinTree {
public:
    BinTree() {
        root = nullptr;
    }

    ~BinTree() {
        if (!root) {
            return;
        }

        std::queue<Node<T> *> buf;
        buf.push(root);

        while (!buf.empty()) {
            Node<T> *tmp = buf.front();
            buf.pop();

            if (tmp->left) {
                buf.push(tmp->left);
            }
            if (tmp->right) {
                buf.push(tmp->right);
            }

            delete(tmp);
        }
    }

    void Insert(T value) {
        auto *newNode = new Node<T>(value);

        if (!root) {
            root = newNode;
            return;
        }

        Node<T> *tmp = root;

        while (true) {
            if (value >= tmp->value) {
                if (!tmp->right) {
                    tmp->right = newNode;
                    return;
                }

                tmp = tmp->right;
            } else {
                if (!tmp->left) {
                    tmp->left = newNode;
                    return;
                }

                tmp = tmp->left;
            }
        }
    }

    void Print(std::ostream &out) {
        if (!root) {
            return;
        }

        std::stack<Node<T> *> buf;
        buf.push(root);

        while (!buf.empty()) {
            Node<T> *tmp = buf.top();
            buf.pop();

            out << *tmp << ' ';

            if (tmp->right) {
                buf.push(tmp->right);
            }
            if (tmp->left) {
                buf.push(tmp->left);
            }
        }
    }
private:
    Node<T> *root;
};

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    in >> n;

    BinTree<int> binTree = BinTree<int>();

    for (int i = 0; i < n; i++) {
        int tmp = 0;
        in >> tmp;
        binTree.Insert(tmp);
    }

    binTree.Print(out);
    out << std::endl;
}

void test() {
    {
        std::stringstream input;
        input << "10" << std::endl;
        input << "2" << std::endl;
        input << "5" << std::endl;
        input << "6" << std::endl;
        input << "4" << std::endl;
        input << "7" << std::endl;
        input << "8" << std::endl;
        input << "9" << std::endl;
        input << "3" << std::endl;
        input << "1" << std::endl;
        input << "10" << std::endl;

        std::stringstream output;

        run(input, output);

        std::cout << "Output:\n" << output.str();

        std::stringstream answer;
        answer << "2 1 5 4 3 6 7 8 9 10 " << std::endl;

        std::cout << "Answer:\n" << answer.str();

        assert(output.str() == answer.str());
    }

    {
        std::stringstream input;
        input << "10" << std::endl;
        input << "1" << std::endl;
        input << "5" << std::endl;
        input << "10" << std::endl;
        input << "2" << std::endl;
        input << "4" << std::endl;
        input << "3" << std::endl;
        input << "8" << std::endl;
        input << "9" << std::endl;
        input << "7" << std::endl;
        input << "6" << std::endl;

        std::stringstream output;

        run(input, output);

        std::cout << "Output:\n" << output.str();

        std::stringstream answer;
        answer << "1 5 2 4 3 10 8 7 6 9 " << std::endl;

        std::cout << "Answer:\n" << answer.str();

        assert(output.str() == answer.str());
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}