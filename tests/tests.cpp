#include <iostream>
#include <cassert>
#include <ctime>
#include <string>
#include "../src/core/HashTable.h" 

// --- Функция замера времени ---
double measureTime(void (*func)(int), int iterations) {
    clock_t start = clock();
    func(iterations);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

void printTableHeader(const char* title) {
    std::cout << std::endl << "=== " << title << " ===" << std::endl;
    std::cout << "+------------+----------------+----------------+" << std::endl;
    std::cout << "| N          | Raw Map (ms)*  | HashTable (ms) |" << std::endl;
    std::cout << "+------------+----------------+----------------+" << std::endl;
}

void printTableRow(int n, double rawTime, double smartTime) {
    std::cout << "| " << n;
    int tmp = n, len = 0;
    while (tmp > 0) { ++len; tmp /= 10; }
    for (int i = len; i < 10; ++i) std::cout << ' ';
    
    std::cout << " | " << rawTime;
    int rawInt = (int)rawTime;
    len = 0; tmp = rawInt > 0 ? rawInt : 1;
    while (tmp > 0) { ++len; tmp /= 10; }
    if (rawTime < 1) len = 3;
    for (int i = len; i < 14; ++i) std::cout << ' ';
    
    std::cout << " | " << smartTime;
    int smartInt = (int)smartTime;
    len = 0; tmp = smartInt > 0 ? smartInt : 1;
    while (tmp > 0) { ++len; tmp /= 10; }
    if (smartTime < 1) len = 3;
    for (int i = len; i < 14; ++i) std::cout << ' ';
    std::cout << " |" << std::endl;
}

// --- Функциональные тесты ---
void testFunctional() {
    std::cout << "=== Running Functional Tests ===" << std::endl;
    
    // 1. Insert & Get
    {
        HashTable<int, std::string> ht;
        ht.insert(1, "One");
        ht.insert(2, "Two");
        assert(ht.get(1) == "One");
        assert(ht.get(2) == "Two");
        std::cout << "[OK] Insert & Get" << std::endl;
    }

    // 2. Update
    {
        HashTable<int, int> ht;
        ht.insert(1, 10);
        ht.insert(1, 20);
        assert(ht.get(1) == 20);
        std::cout << "[OK] Update value" << std::endl;
    }

    // 3. Contains & Remove
    {
        HashTable<std::string, int> ht;
        ht.insert("key", 100);
        assert(ht.contains("key"));
        ht.remove("key");
        assert(!ht.contains("key"));
        std::cout << "[OK] Contains & Remove" << std::endl;
    }

    // 4. Resize stress
    {
        HashTable<int, int> ht(2); // Маленький начальный размер
        for (int i = 0; i < 1000; ++i) {
            ht.insert(i, i * 10);
        }
        assert(ht.size() == 1000);
        assert(ht.get(999) == 9990);
        std::cout << "[OK] Auto-resize" << std::endl;
    }

    std::cout << "=== All Functional Tests Passed! ===" << std::endl << std::endl;
}

// --- Нагрузочные тесты ---
void bench_ht_insert(int n) {
    HashTable<int, int> ht;
    for (int i = 0; i < n; ++i) ht.insert(i, i);
}

void bench_ht_get(int n) {
    HashTable<int, int> ht;
    for (int i = 0; i < n; ++i) ht.insert(i, i);
    for (int i = 0; i < n; ++i) ht.get(i);
}

void bench_ht_remove(int n) {
    HashTable<int, int> ht;
    for (int i = 0; i < n; ++i) ht.insert(i, i);
    for (int i = 0; i < n; ++i) ht.remove(i);
}

// Для сравнения используем простую реализацию на массиве пар (линейный поиск)
// так как std::map/std::unordered_map запрещены или слишком сложны для сравнения "с нуля"
struct SimplePair { int key; int val; };
void bench_raw_insert(int n) {
    SimplePair* arr = new SimplePair[n];
    int count = 0;
    for (int i = 0; i < n; ++i) {
        arr[count++] = {i, i};
    }
    delete[] arr;
}

void bench_raw_get(int n) {
    SimplePair* arr = new SimplePair[n];
    for (int i = 0; i < n; ++i) arr[i] = {i, i};
    
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (arr[j].key == i) {
                sum += arr[j].val;
                break;
            }
        }
    }
    (void)sum; //"я знаю, что sum не используется дальше"
    delete[] arr;
}

void testPerformance() {
    std::cout << "=== Performance Benchmarks ===" << std::endl;
    const int sizes[] = { 1000, 5000, 10000, 50000 };
    const int numSizes = 4;

    // Таблица 1: Insert
    printTableHeader("Table 1: Insert Operation");
    for (int i = 0; i < numSizes; ++i) {
        int n = sizes[i];
        double rawTime = measureTime(bench_raw_insert, n);
        double smartTime = measureTime(bench_ht_insert, n);
        printTableRow(n, rawTime, smartTime);
    }
    std::cout << "+------------+----------------+----------------+" << std::endl;

    // Таблица 2: Get (Search)
    printTableHeader("Table 2: Get Operation (Search)");
    for (int i = 0; i < numSizes; ++i) {
        int n = sizes[i];
        double rawTime = measureTime(bench_raw_get, n);
        double smartTime = measureTime(bench_ht_get, n);
        printTableRow(n, rawTime, smartTime);
    }
    std::cout << "+------------+----------------+----------------+" << std::endl;
    
    // Таблица 3: Remove
    printTableHeader("Table 3: Remove Operation");
    for (int i = 0; i < numSizes; ++i) {
        int n = sizes[i];
        // Для raw remove просто замеряем удаление из массива (O(1) если не сдвигать)
        double rawTime = measureTime([](int n){ delete[] new int[n]; }, n); 
        double smartTime = measureTime(bench_ht_remove, n);
        printTableRow(n, rawTime, smartTime);
    }
    std::cout << "+------------+----------------+----------------+" << std::endl;
}

void runAllTests() {
    testFunctional();
    testPerformance();
}