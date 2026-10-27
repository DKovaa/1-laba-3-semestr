#ifndef LINKED_LIST_TPP
#define LINKED_LIST_TPP

#include "LinkedList.h"

template <class T>
LinkedList<T>::Node::Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

template <class T>
LinkedList<T>::ListEnumerator::ListEnumerator(const LinkedList<T>* l) 
    : list(l), current(nullptr), pos(-1) {}

template <class T>
bool LinkedList<T>::ListEnumerator::MoveNext() {
    pos++;
    
    if (pos == 0) {
        current = list->head;
    } else if (current != nullptr) {
        current = current->next;
    }
    
    if (current != nullptr) {
        return true;
    }
    return false;
}

template <class T>
const T& LinkedList<T>::ListEnumerator::GetCurrent() const {
    return current->data;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::GetNodeAt(int index) const {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index out of range");
    }
    
    Node* current;
    if (index < length / 2) {
        current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = tail;
        for (int i = length - 1; i > index; i--) {
            current = current->prev;
        }
    }
    return current;
}

template <class T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), tail(nullptr), length(0) {
    if (count < 0) throw std::invalid_argument("Count cannot be negative");
    if (count > 0 && items == nullptr) throw std::invalid_argument("Items array is null");
    
    for (int i = 0; i < count; i++) {
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), length(0) {
    Node* current = list.head;
    while (current != nullptr) {
        Append(current->data);
        current = current->next;
    }
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        Clear(); // Очищаем текущий список
        Node* current = other.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <class T>
LinkedList<T>::~LinkedList() {
    Clear();
}

template <class T>
T LinkedList<T>::GetFirst() const {
    if (length == 0) {
        throw std::out_of_range("List is empty");
    }
    return head->data;
}

template <class T>
T LinkedList<T>::GetLast() const {
    if (length == 0) {
        throw std::out_of_range("List is empty");
    }
    return tail->data;
}

template <class T>
T LinkedList<T>::Get(int index) const {
    return GetNodeAt(index)->data;
}

template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= length || startIndex > endIndex) {
        throw std::out_of_range("Invalid indices");
    }

    LinkedList<T>* subList = new LinkedList<T>();
    Node* current = GetNodeAt(startIndex);
    
    for (int i = startIndex; i <= endIndex; i++) {
        subList->Append(current->data);
        current = current->next;
    }
    
    return subList;
}

template <class T>
int LinkedList<T>::GetLength() const {
    return length;
}

template <class T>
void LinkedList<T>::Append(T item) {
    Node* newNode = new Node(item);
    
    if (length == 0) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    length++;
}

template <class T>
void LinkedList<T>::Prepend(T item) {
    Node* newNode = new Node(item);
    
    if (length == 0) {
        head = tail = newNode;
    } else {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
    }
    length++;
}

template <class T>
void LinkedList<T>::InsertAt(T item, int index) {
    if (index < 0 || index > length) {
        throw std::out_of_range("Index out of range");
    }
    
    if (index == 0) {
        Prepend(item);
    } else if (index == length) {
        Append(item);
    } else {
        Node* current = GetNodeAt(index);
        Node* newNode = new Node(item);
        
        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
        
        length++;
    }
}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) {
    LinkedList<T>* result = new LinkedList<T>(*this);
    
    Node* current = list->head;
    while (current != nullptr) {
        result->Append(current->data);
        current = current->next;
    }
    
    return result;
}

template <class T>
void LinkedList<T>::Clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    length = 0;
}

template <class T>
IEnumerator<T>* LinkedList<T>::GetEnumerator() const {
    return new ListEnumerator(this);
}

// Явное инстанциирование
template class LinkedList<int>;
template class LinkedList<double>;
template class LinkedList<std::string>;

#endif