#include <iostream>
#include <string>
#include <ctime>
#include "../src/core/BinaryTree.h"
#include "../src/containers/DynamicArray.h"

// Объявление функций тестов из tests.cpp
void runAllTests();

// Вспомогательная функция для вывода дерева в консоль
void printInt(const int& val) {
    std::cout << val << " ";
}

// Функция бенчмарка для графика
void runBenchmark(DynamicArray<int>& outSizes, DynamicArray<double>& outTimes) {
    int testSizes[] = {1000, 5000, 10000, 50000, 100000};
    int n = 5;
    
    outSizes.Resize(n);
    outTimes.Resize(n);

    std::cout << "Running Benchmark..." << std::endl;
    for (int i = 0; i < n; ++i) {
        BinaryTree<int> tree;
        int size = testSizes[i];
        
        clock_t start = clock();
        for (int j = 0; j < size; ++j) {
            tree.insert(j);
        }
        clock_t end = clock();
        
        double ms = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
        
        outSizes.Set(i, size);
        outTimes.Set(i, ms);
        
        std::cout << "N=" << size << ": " << ms << " ms" << std::endl;
    }
}

void printMenu() {
    std::cout << "\n=== Binary Tree Console Interface ===\n";
    std::cout << "1. Insert Value\n";
    std::cout << "2. Find Value\n";
    std::cout << "3. Show Tree (InOrder/KLP)\n";
    std::cout << "4. Run Auto Tests\n";
    std::cout << "5. Run Benchmark\n";
    std::cout << "6. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    BinaryTree<int> tree;
    int choice;
    
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;
        
        if (choice == 1) {
            int val;
            std::cout << "Enter integer: ";
            std::cin >> val;
            tree.insert(val);
            std::cout << "Inserted.\n";
        } 
        else if (choice == 2) {
            int val;
            std::cout << "Find: ";
            std::cin >> val;
            std::cout << (tree.contains(val) ? "Found!\n" : "Not found.\n");
        }
        else if (choice == 3) {
            std::cout << "Tree (KLP): ";
            tree.traverseKLP(printInt);
            std::cout << std::endl;
        }
        else if (choice == 4) {
            runAllTests();
        }
        else if (choice == 5) {
            DynamicArray<int> sizes(5);
            DynamicArray<double> times(5);
            runBenchmark(sizes, times);
        }
        else if (choice == 6) {
            break;
        }
    }
    return 0;
}