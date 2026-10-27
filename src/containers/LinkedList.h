#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdexcept>
#include <string>
#include "../interfaces/IEnumerator.h"

template <class T>
class LinkedList : public IEnumerable<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        
        Node(const T& value);
    };

    Node* head;
    Node* tail;
    int length;

    Node* GetNodeAt(int index) const;

    class ListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<T>* list;
        Node* current;
        int pos;
    public:
        explicit ListEnumerator(const LinkedList<T>* l);
        bool MoveNext() override;
        const T& GetCurrent() const override;
    };

public:
    LinkedList(T* items, int count);
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    LinkedList& operator=(const LinkedList<T>& other);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    LinkedList<T>* Concat(LinkedList<T>* list);
    void Clear();

    IEnumerator<T>* GetEnumerator() const override;
};

#include "LinkedList.tpp"

#endif