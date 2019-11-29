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


template <typename T, class Comparator = std::less<T>>
class AvlTree {
    struct Node {
        T value;
        Node *left;
        Node *right;
        unsigned char height;
        unsigned int quantity;

        explicit Node(const T &value) : value(value), left(nullptr), right(nullptr), height(1), quantity(1) {}
    };
public:
    AvlTree() : root(nullptr), cmp(Comparator()) {}
    ~AvlTree() {
        destroyTree(root);
    }

    unsigned int Add(const T &value) {
        unsigned int position = 0;
        root = addInternal(root, value, position);
        return position;
    }
    void Remove(const int position) {
        root = removeInternal(root, position);
    }

private:
    Node *root;
    Comparator cmp;

    Node* removeInternal(Node *node, const unsigned int position) {
        if (!node) {
            return nullptr;
        }

        unsigned int current_pos = getQuantity(node->right);
        if (cmp(current_pos, position)) {
            node->left = removeInternal(node->left, position - current_pos - 1);
        }
        else if (current_pos != position) {
            node->right = removeInternal(node->right, position);
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
                max->left = removeMax(left);
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

    Node* addInternal(Node *node, const T &value, unsigned int &position) {
        if (!node) {
            return new Node(value);
        }

        node->quantity++;
        if (cmp(node->value, value) || node->value == value) {
            node->right = addInternal(node->right, value, position);
        }
        else {
            position += getQuantity(node->right) + 1;
            node->left = addInternal(node->left, value, position);
        }

        return doBalance(node);
    }

    Node* doBalance(Node *node) {
        fixHeight(node);
        fixQuantity(node);

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

    void fixQuantity(Node *node) {
        node->quantity = getQuantity(node->left) + getQuantity(node->right) + 1;
    }

    unsigned char getHeight(Node *node) {
        return node ? node->height : 0;
    }

    unsigned int getQuantity(Node *node) {
        return node ? node->quantity : 0;
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixHeight(node);
        fixHeight(tmp);
        fixQuantity(node);
        fixQuantity(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixHeight(node);
        fixHeight(tmp);
        fixQuantity(node);
        fixQuantity(tmp);

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
    AvlTree<int> tree = AvlTree<int>();

    int n = 0;
    in >> n;

    int op = 0;
    int val = 0;

    for (int i = 0; i < n; i++) {
        in >> op >> val;
        switch (op) {
            case 1:
                out << tree.Add(val) << std::endl;
                break;
            case 2:
                tree.Remove(val);
                break;
            default:
                break;
        }
    }
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
    {
        std::stringstream input;
        input << "7" << std::endl;
        input << "1 100" << std::endl;
        input << "1 200" << std::endl;
        input << "1 150" << std::endl;
        input << "1 50" << std::endl;
        input << "1 300" << std::endl;
        input << "2 1" << std::endl;
        input << "1 170" << std::endl;

        std::stringstream output;

        run(input, output);

        std::cout << "Output:\n" << output.str();

        std::stringstream answer;
        answer << "0" << std::endl;
        answer << "0" << std::endl;
        answer << "1" << std::endl;
        answer << "3" << std::endl;
        answer << "0" << std::endl;
        answer << "1" << std::endl;

        std::cout << "Answer:\n" << answer.str();

        assert(output.str() == answer.str());
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}