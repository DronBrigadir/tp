// В одной военной части решили построить в одну шеренгу по росту.
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
// Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту –
// сначала самые высокие, а в конце – самые низкие.
// За расстановку солдат отвечал прапорщик, который заметил интересную особенность –
// все солдаты в части разного роста. Ваша задача состоит в том,
// чтобы помочь прапорщику правильно расставлять солдат,
// а именно для каждого приходящего солдата указывать,
// перед каким солдатом в строе он должен становится.
// Требуемая скорость выполнения команды - O(log n).

// Формат ввода:
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
// В каждой следующей строке содержится описание команды:
// число 1 и X если солдат приходит в строй (X – рост солдата,
// натуральное число до 100 000 включительно) и число 2 и Y если солдата,
// стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.

// Формат вывода:
// На каждую команду 1 (добавление в строй) вы должны выводить число K
// – номер позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).

#include <iostream>
#include <sstream>
#include <cassert>


template <typename T>
class AvlTree {
    struct Node {
        T value;
        Node *left;
        Node *right;
        unsigned char height;

        explicit Node(const T &value) : value(value), left(nullptr), right(nullptr), height(1) {}
    };
public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() {
        destroyTree(root);
    }

    bool Has(const T &value) {
        Node *tmp = root;
        while (tmp) {
            if (tmp->value == value)
                return true;

            if (tmp->value < value) {
                tmp = tmp->right;
            }
            else {
                tmp = tmp->left;
            }
        }
        return false;
    }

    void Add(const T &value) {
        root = addInternal(root, value);
    }
    void Remove(const T &value) {
        root = removeInternal(root, value);
    }

private:
    Node *root;

    Node* removeInternal(Node *node, const T &value) {
        if (!node) {
            return nullptr;
        }

        if (node->value < value) {
            node->right = removeInternal(node->right, value);
        }
        else if (node->value > value) {
            node->left = removeInternal(node->left, value);
        }
        else {
            Node *left = node->left;
            Node *right = node->right;
            delete node;

            if (!left && !right) {
                return nullptr;
            }

            if (getHeight(left) > getHeight(right)) {
                Node *max = findMax(left);
                max-left = removeMax(left);
                max->right = right;

                return doBalance(max);
            } else {
                Node *min = findMin(right);
                min->right = removeMin(right);
                min->left = left;

                return doBalance(min);
            }
        }

        return doBalance(node);
    }

    Node* findMin(Node *node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* findMax(Node *node) {
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    Node* removeMin(Node *node) {
        if (!node->left) {
            return node->right;
        }
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node* removeMax(Node *node) {
        if (!node->right) {
            return node->left;
        }
        node->right = removeMax(node->right);
        return doBalance(node);
    }

    Node* addInternal(Node *node, const T &value) {
        if (!node) {
            return new Node(value);
        }

        if (node->value <= value) {
            node->right = addInternal(node->right, value);
        }
        else {
            node->left = addInternal(node->left, value);
        }

        return doBalance(node);
    }

    Node* doBalance(Node *node) {
        fixHeight(node);

        switch (getBalance(node)) {
            case 2:
            {
                if (getBalance(node->right) < 0) {
                    node->right = rotateRight(node->right);
                }
                return rotateLeft(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0) {
                    node->left = rotateLeft(node->left);
                }
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    void fixHeight(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    unsigned char getHeight(Node *node) {
        return node ? node->height : 0;
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    void destroyTree(Node *node) {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

void run(std::istream &in, std::ostream &out) {
}

void test() {
    {
        std::stringstream input;
        input << "5" << std::endl;
        input << "1 100" << std::endl;
        input << "1 200" << std::endl;
        input << "1 50" << std::endl;
        input << "2 1" << std::endl;
        input << "1 150" << std::endl;

        std::stringstream output;

        run(input, output);

        std::cout << "Output:\n" << output.str();

        std::stringstream answer;
        answer << "0" << std::endl;
        answer << "0" << std::endl;
        answer << "2" << std::endl;
        answer << "1" << std::endl;

        std::cout << "Answer:\n" << answer.str();

        assert(output.str() == answer.str());
    }
}

int main () {
    test();
    //run(std::cin, std::cout);
    return 0;
}