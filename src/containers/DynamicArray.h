#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include <string>
#include "../interfaces/IEnumerator.h"

template <class T>
class DynamicArray : public IEnumerable<T> {
private:
    T* data;
    int size;

    class ArrayEnumerator : public IEnumerator<T> {
    private:
        const DynamicArray<T>* arr;
        int pos;
    public:
        explicit ArrayEnumerator(const DynamicArray<T>* a);
        bool MoveNext() override;
        const T& GetCurrent() const override;
    };

public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& dynamicArray);
    DynamicArray& operator=(const DynamicArray<T>& other);
    ~DynamicArray();

    T Get(int index) const;
    T& GetRef(int index);
    int GetSize() const;
    void Set(int index, T value);
    void Resize(int newSize);

    IEnumerator<T>* GetEnumerator() const override;
};

#include "DynamicArray.tpp"

#endif