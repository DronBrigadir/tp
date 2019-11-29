#include <string>
#include <iostream>
#include <map>
#include <queue>
#include <memory>
#include <bitset>
#include <stack>

//#include "Huffman.h"

typedef unsigned char byte;

#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

class BitWriter
{
public:
    BitWriter() : bitCount(0) {}

    void WriteBit(byte bit) {
        if (bitCount % 8 == 0)
            buffer.push_back(0);
        if (bit)
        {
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
        }
        bitCount++;
    }

    void WriteByte(byte byteVal) {
        if (bitCount % 8 == 0)
            buffer.push_back(byteVal);
        else
        {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= byteVal >> offset;
            buffer.push_back(byteVal << (8 - offset));
        }
        bitCount += 8;
    }

    std::vector<byte> GetBuffer() const {
        return buffer;
    }

    size_t GetBitCount() const {
        return bitCount;
    }
private:
    std::vector<byte> buffer;
    size_t bitCount;
};

void visualize(const std::vector<byte> &buffer) {
    for (auto &byte: buffer) {
        std::cout << std::bitset<8>(byte) << "|";
    }
    std::cout << std::endl;
}

struct Node {
    size_t freq;
    byte value;
    Node *left;
    Node *right;
    std::string code;

    explicit Node(const size_t freq, const byte value) :
        freq(freq), value(value), left(nullptr), right(nullptr), code("") {}

    Node() : freq(0), value(0), left(nullptr), right(nullptr), code("") {}

    Node(const Node &rhs) {
        freq = rhs.freq;
        value = rhs.value;
        code = rhs.code;
        if (!rhs.left) {
            left = nullptr;
        } else {
            left = new Node(*rhs.left);
        }

        if (!rhs.right) {
            right = nullptr;
        } else {
            right = new Node(*rhs.right);
        }
    }

    bool operator==(const Node &rhs) const {
        return freq == rhs.freq;
    }

    bool operator<(const Node &rhs) const {
        return freq < rhs.freq;
    }

    bool operator>(const Node &rhs) const {
        return freq > rhs.freq;
    }

    Node& operator=(const Node& rhs) {
        delete(left);
        delete(right);
        freq = rhs.freq;
        value = rhs.value;
        code = rhs.code;
        if (!rhs.left) {
            left = nullptr;
        } else {
            left = new Node(*rhs.left);
        }

        if (!rhs.right) {
            right = nullptr;
        } else {
            right = new Node(*rhs.right);
        }
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, const Node &rhs) {
    os << "Frequency: " << rhs.freq << " , value: " << rhs.value << std::endl;
    return os;
}

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
    size_t GetSize() {
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
    size_t size_;
    size_t curSize_;
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

class MyException
{
public:
    explicit MyException(const std::string &error) : mError(error) {}
    std::string GetError() {
        return mError;
    }

private:
    std::string mError;
};

template <typename T, class Comparator = std::less<T>>
class Heap {
public:
    Heap() : comparator_(Comparator()) {}

    ~Heap() {}
    void Insert(T elem) {
        array_.Add(elem);
        SiftUp(array_.GetSize() - 1);
    }
    T ExtractRoot() {
        if (array_.GetSize() == 0) {
            throw MyException("Heap is empty");
        }

        T result = array_[0];

        array_[0] = array_[array_.GetSize() - 1];
        array_.DeleteLast();

        if (array_.GetSize() != 0) {
            SiftDown(0);
        }

        return result;
    }
    T PeekRoot() {
        if (array_.GetSize() == 0) {
            throw MyException("Heap is empty");
        }
        return array_[0];
    }
    size_t GetSize() {
        return array_.GetSize();
    }
private:
    Array<T> array_;
    Comparator comparator_;
    void SiftUp(size_t index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comparator_(array_[parent], array_[index]) || array_[parent] == array_[index]) {
                return;
            }

            std::swap(array_[index], array_[parent]);
            index = parent;
        }
    }
    void SiftDown(size_t index) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;

        size_t largest = index;

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

std::map<unsigned char, size_t> FreqCalc(IInputStream &src, std::unique_ptr<std::queue<byte>> &dst) {
    byte value;
    std::map<byte, size_t> tmp;

    while(src.Read(value)) {
        dst->push(value);
        tmp[value]++;
    }

    return tmp;
}

void MakeEncodeTree(std::unique_ptr<Heap<Node>> &heap, const std::map<byte, size_t> &src) {
    for (auto iter : src) {
        Node tmp = Node(iter.second, iter.first);
        heap->Insert(tmp);
    }

    while (heap->GetSize() != 1) {
        Node first = heap->ExtractRoot();
        Node second = heap->ExtractRoot();

        Node newNode = Node(first.freq + second.freq, 0);
        newNode.left = new Node(first);
        newNode.right = new Node(second);

        heap->Insert(newNode);
    }
}

void createCodedTable(std::unique_ptr<std::map<byte, std::string>> &table, Node &root) {
    std::stack<Node*> buf;
    buf.push(&root);

    while (!buf.empty()) {
        Node *tmp = buf.top();
        buf.pop();

        if (!tmp->right && !tmp->left) {
            (*table)[tmp->value] = tmp->code;
        }
        if (tmp->right) {
            tmp->right->code += tmp->code + "1";
            buf.push(tmp->right);
        }
        if (tmp->left) {
            tmp->left->code += tmp->code + "0";
            buf.push(tmp->left);
        }
    }
}

void WriteTree(std::shared_ptr<BitWriter> &dst, Node *node) {
    if (!node->right && !node->left) {
        dst->WriteBit(1);
        dst->WriteByte(node->value);
        return;
    }
    if (node->left) {
        WriteTree(dst, node->left);
    }
    if (node->right) {
        WriteTree(dst, node->right);
    }

    dst->WriteBit(0);
}

void WriteSequence(std::shared_ptr<BitWriter> &dst, std::unique_ptr<std::queue<byte>> &src,
                   std::unique_ptr<std::map<byte, std::string>> &table) {
    while (!src->empty()) {
        byte tmp = src->front();
        src->pop();

        std::string code = (*table)[tmp];

        for (size_t i = 0; i < code.size(); i++) {
            if (code[i] == '0') {
                dst->WriteBit(0);
            } else {
                dst->WriteBit(1);
            }
        }
    }
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    // Копирование входных данных
    auto originalCopy = std::make_unique<std::queue<byte>>();

    // Определение частотности символов
    std::map<byte, size_t> byteFreq = FreqCalc(original, originalCopy);

    size_t numberOfChars = byteFreq.size();

    // Получение дерева кодирование
    auto encodingTree = std::make_unique<Heap<Node>>();
    MakeEncodeTree(encodingTree, byteFreq);

    // Получение таблицы соответсвие кодов и символов
    auto codedTable = std::make_unique<std::map<byte, std::string>>();
    Node root = encodingTree->ExtractRoot();
    createCodedTable(codedTable, root);

    // Записываем дерево
    auto bw = std::make_shared<BitWriter>();
    bw->WriteByte(numberOfChars);
    WriteTree(bw, &root);

    // Записываем последовательность
    WriteSequence(bw, originalCopy, codedTable);

    // Записываем количество значащих бит в последнем байте (пишем в байт за последним)
    unsigned char numberOfSignificantBits = bw->GetBitCount() % 8;
    // 0 означает 8 значащих битов
    numberOfSignificantBits = numberOfSignificantBits == 0 ? 8 : numberOfSignificantBits;
    for (int i = 0; i < 8 - numberOfSignificantBits; i++) {
        bw->WriteBit(0);
    }
    bw->WriteByte(numberOfSignificantBits);

    auto sequence = bw->GetBuffer();
    for (auto &byte : sequence) {
        compressed.Write(byte);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) {

}

int main () {
    std::map<byte , size_t> byteFreq;
    byteFreq['a'] = 5;
    byteFreq['b'] = 2;
    byteFreq['c'] = 1;
    byteFreq['d'] = 1;
    byteFreq['r'] = 2;

    size_t numberOfChars = byteFreq.size();

    for (auto iter : byteFreq) {
        std::cout << iter.first << ' ' << iter.second << std::endl;
    }

    std::cout << "Number of chars: " << byteFreq.size() << std::endl;

    auto encodingTree = std::make_unique<Heap<Node>>();
    MakeEncodeTree(encodingTree, byteFreq);

    auto codedTable = std::make_unique<std::map<byte, std::string>>();
    Node root = encodingTree->ExtractRoot();
    createCodedTable(codedTable, root);

    for (auto iter : *codedTable) {
        std::cout << iter.first << ' ' << iter.second << std::endl;
    }

    auto bw = std::make_shared<BitWriter>();
    bw->WriteByte(numberOfChars);

    WriteTree(bw, &root);

    visualize(bw->GetBuffer());

    std::cout << "a: " << std::bitset<8>('a') << std::endl;
    std::cout << "b: " << std::bitset<8>('b') << std::endl;
    std::cout << "c: " << std::bitset<8>('c') << std::endl;
    std::cout << "d: " << std::bitset<8>('d') << std::endl;
    std::cout << "r: " << std::bitset<8>('r') << std::endl;

    auto originalCopy = std::make_unique<std::queue<byte>>();
    originalCopy->push('a');
    originalCopy->push('b');
    originalCopy->push('r');
    originalCopy->push('a');
    originalCopy->push('c');
    originalCopy->push('a');
    originalCopy->push('d');
    originalCopy->push('a');
    originalCopy->push('b');
    originalCopy->push('r');
    originalCopy->push('a');

    WriteSequence(bw, originalCopy, codedTable);

    visualize(bw->GetBuffer());

    unsigned char numberOfSignificantBits = bw->GetBitCount() % 8;
    numberOfSignificantBits = numberOfSignificantBits == 0 ? 8 : numberOfSignificantBits;

    std::cout << "true number: " << numberOfSignificantBits << std::endl;

    for (int i = 0; i < 8 - numberOfSignificantBits; i++) {
        bw->WriteBit(0);
    }
    bw->WriteByte(numberOfSignificantBits);

    visualize(bw->GetBuffer());
}