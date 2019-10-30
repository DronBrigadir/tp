// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество,
// удаления строки из множества и проверки принадлежности данной строки множеству.
// Для разрешения коллизий использовать двойное хеширование.
// Формат входных данных:
// Каждая строка входных данных задает одну операцию над множеством.
// Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
// Тип операции  – один из трех символов:
// +  означает добавление данной строки в множество;
// -  означает удаление  строки из множества;
// ?  означает проверку принадлежности данной строки множеству.
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
// При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
// Формат выходных данных:
// Программа должна вывести для каждой операции одну из двух строк OK или FAIL,
// в зависимости от того, встречается ли данное слово в нашем множестве.

#define DEFAULT_SIZE 8
const double ALPHA = 3.0 / 4.0;

#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <cstddef>

template <typename T>
struct CellVal {
    T key;
    bool deleted;
    bool empty;

    CellVal(T &val): key(val), deleted(false), empty(false) {}
    CellVal(): key(), deleted(false), empty(true) {}
};

size_t HashMain(const std::string &s, size_t size)
{
    size_t hash = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        hash = (hash * 73 + s[i]) % size;
    }

    return hash;
}

size_t HashEx(const std::string &s, size_t size)
{
    size_t hash = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        hash = (hash * 179 + s[i]) % size;
    }

    return hash;
}

template <typename T>
class HashTable {
public:
    HashTable(size_t tableSize): table(tableSize, CellVal<T>()), size(0) {}
    ~HashTable() {}
    void Add(const T &key) {
        if (size >= table.capacity() * ALPHA) {
            Expand();
        }

        size++;

        size_t hashMain = HashMain(key, table.capacity());
        size_t hashEx = HashEx(key, table.capacity());
        size_t hash = hashMain;

        unsigned int probNumber = 1;

        while (table.capacity() > probNumber) {
            if (table[hash].empty) {
                T tmp = key;
                table[hash] = CellVal<T>(tmp);
                return;
            }
            else {
                hash = (hashMain + probNumber * (2 * hashEx + 1)) % table.capacity();
                probNumber++;
            }
        }
    }
    bool Delete(const T &key) {
        size_t hashMain = HashMain(key, table.capacity());
        size_t hashEx = HashEx(key, table.capacity());
        size_t hash = hashMain;

        unsigned int probNumber = 1;

        while (table.capacity() > probNumber) {
            if (!table[hash].deleted && !table[hash].empty && table[hash].key == key) {
                table[hash] = CellVal<T>();
                table[hash].deleted = true;
                size--;
                return true;
            }
            if (table[hash].empty && !table[hash].deleted) {
                return false;
            } else {
                hash = (hashMain + probNumber * (2 * hashEx + 1)) % table.capacity();
                probNumber++;
            }
        }

        return false;
    }
    bool Find(const T &key) {
        size_t hashMain = HashMain(key, table.capacity());
        size_t hashEx = HashEx(key, table.capacity());
        size_t hash = hashMain;

        unsigned int probNumber = 1;

        while (table.capacity() > probNumber) {
            if (!table[hash].deleted && !table[hash].empty && table[hash].key == key) {
                return true;
            }
            if (table[hash].empty && !table[hash].deleted) {
                return false;
            } else {
                hash = (hashMain + probNumber * (2 * hashEx + 1)) % table.capacity();
                probNumber++;
            }
        }

        return false;
    }

private:
    std::vector<CellVal<T>> table;
    size_t size;
    void Expand()
    {
        std::vector<CellVal<T>> newTable(table.capacity() * 2, CellVal<T>());

        for (size_t i = 0; i < table.capacity(); i++)
        {
            T key = table[i].key;

            size_t hashMain = HashMain(key, newTable.capacity());
            size_t hashEx = HashEx(key, newTable.capacity());
            size_t hash = hashMain;

            unsigned int probNumber = 1;

            while (newTable.capacity() > probNumber) {
                if (newTable[hash].empty) {
                    newTable[hash] = CellVal<T>(key);
                    break;
                }
                else {
                    hash = (hashMain + probNumber * (2 * hashEx + 1)) % newTable.capacity();
                    probNumber++;
                }
            }
        }

        table = std::move(newTable);
    }
};

template <typename T>
class Set {
public:
    Set(): hashTable(DEFAULT_SIZE) {}
    ~Set() {}
    bool Add(const T &key) {
        if (hashTable.Find(key)) {
            return false;
        } else {
            hashTable.Add(key);
            return true;
        }
    }
    bool Delete(const T &key) {
        return hashTable.Delete(key);
    }
    bool Find(const T &key) {
        return hashTable.Find(key);
    }

private:
    HashTable<T> hashTable;
};

void run(std::istream &in, std::ostream &out) {
    char op;
    std::string key;

    Set<std::string> set = Set<std::string>();

    while (in >> op >> key)
    {
        switch (op)
        {
            case '?':
            {
                out << (set.Find(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '+':
            {
                out << (set.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-':
            {
                out << (set.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }
}

void test() {
    {
        std::stringstream input;
        input << "+ hello" << std::endl;
        input << "+ bye" << std::endl;
        input << "? bye" << std::endl;
        input << "+ bye" << std::endl;
        input << "- bye" << std::endl;
        input << "? bye" << std::endl;
        input << "? hello" << std::endl;

        std::stringstream output;

        run(input, output);

        std::stringstream answer;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "FAIL" << std::endl;
        answer << "OK" << std::endl;
        answer << "FAIL" << std::endl;
        answer << "OK" << std::endl;

        assert(output.str() == answer.str());
    }
    {
        std::stringstream input;
        input << "+ h" << std::endl;
        input << "+ he" << std::endl;
        input << "+ hel" << std::endl;
        input << "+ hell" << std::endl;
        input << "+ hello" << std::endl;
        input << "+ hello" << std::endl;
        input << "+ b" << std::endl;
        input << "+ by" << std::endl;
        input << "+ bye" << std::endl;
        input << "? bye" << std::endl;
        input << "- bye" << std::endl;
        input << "? bye" << std::endl;
        input << "? hel" << std::endl;
        input << "- hel" << std::endl;
        input << "? hel" << std::endl;

        std::stringstream output;

        run(input, output);

        std::stringstream answer;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "FAIL" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "FAIL" << std::endl;
        answer << "OK" << std::endl;
        answer << "OK" << std::endl;
        answer << "FAIL" << std::endl;

        assert(output.str() == answer.str());
    }
}

int main () {
    //test();
    run(std::cin, std::cout);
    return 0;
}