#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <functional>
#include <stdexcept>
#include "../containers/DynamicArray.h"
#include "../containers/LinkedList.h"
#include "Pair.h"

template <typename K, typename V>
class HashTable {
private:
    DynamicArray< LinkedList< Pair<K, V> > > buckets;
    int capacity;
    int count;
    double loadFactorThreshold;

    // Вычисление индекса корзины
    int getIndex(const K& key) const {
        std::hash<K> hasher;
        return static_cast<int>(hasher(key) % static_cast<size_t>(capacity));
    }

    // Увеличение размера таблицы в 2 раза при переполнении
    void resize() {
        int newCapacity = capacity * 2;
        DynamicArray< LinkedList< Pair<K, V> > > newBuckets(newCapacity);
        
        for (int i = 0; i < capacity; ++i) {
            LinkedList< Pair<K, V> >& list = buckets.GetRef(i);
            IEnumerator< Pair<K, V> >* enumerator = list.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                const Pair<K, V>& pair = enumerator->GetCurrent();
                int newIndex = static_cast<int>(std::hash<K>()(pair.first) % static_cast<size_t>(newCapacity));
                newBuckets.GetRef(newIndex).Append(pair);
            }
            delete enumerator;
        }
        
        buckets = newBuckets;
        capacity = newCapacity;
    }

public:
    explicit HashTable(int initialCapacity = 16) 
        : buckets(initialCapacity), 
          capacity(initialCapacity), 
          count(0), 
          loadFactorThreshold(0.75) {
    }

    // Безопасное получение значения (без исключений)
    bool try_get(const K& key, V& out_value) const {
        int index = getIndex(key);
        const LinkedList< Pair<K, V> >& list = buckets.Get(index);
        
        IEnumerator< Pair<K, V> >* enumerator = list.GetEnumerator();
        bool found = false;
        
        while (enumerator->MoveNext()) {
            if (enumerator->GetCurrent().first == key) {
                out_value = enumerator->GetCurrent().second;
                found = true;
                break;
            }
        }
        delete enumerator;
        return found;
    }

    // Получение значения по ключу (с исключением)
    V get(const K& key) {
        V value;
        if (try_get(key, value)) {
            return value;
        }
        throw std::out_of_range("Key not found in HashTable");
    }

    // Проверка наличия ключа
    bool contains(const K& key) const {
        V dummy;
        return try_get(key, dummy);
    }

    // Вставка или обновление значения по ключу
    void insert(const K& key, const V& value) {
        // Проверка на необходимость расширения таблицы
        if (static_cast<double>(count + 1) / capacity > loadFactorThreshold) {
            resize();
        }

        int index = getIndex(key);
        LinkedList< Pair<K, V> >& list = buckets.GetRef(index);
        
        // Проверяем, существует ли уже такой ключ
        IEnumerator< Pair<K, V> >* enumerator = list.GetEnumerator();
        bool keyExists = false;
        
        while (enumerator->MoveNext()) {
            if (enumerator->GetCurrent().first == key) {
                keyExists = true;
                break;
            }
        }
        delete enumerator;
        
        if (keyExists) {
            // Ключ существует: пересобираем список, исключая старую пару
            LinkedList< Pair<K, V> > newList;
            enumerator = list.GetEnumerator();
            
            while (enumerator->MoveNext()) {
                if (enumerator->GetCurrent().first != key) {
                    newList.Append(enumerator->GetCurrent());
                }
            }
            delete enumerator;
            
            list = newList; // Заменяем старый список на обновлённый
        } else {
            ++count; // Увеличиваем счётчик только при добавлении нового ключа
        }

        // Добавляем актуальную пару в конец списка
        list.Append(Pair<K, V>(key, value));
    }

    // Удаление элемента по ключу
    bool remove(const K& key) {
        // Сначала проверяем наличие, чтобы избежать лишней работы
        if (!contains(key)) {
            return false;
        }

        int index = getIndex(key);
        LinkedList< Pair<K, V> >& list = buckets.GetRef(index);
        
        // Пересобираем список без удаляемого элемента
        LinkedList< Pair<K, V> > newList;
        IEnumerator< Pair<K, V> >* enumerator = list.GetEnumerator();
        
        while (enumerator->MoveNext()) {
            if (enumerator->GetCurrent().first != key) {
                newList.Append(enumerator->GetCurrent());
            }
        }
        delete enumerator;
        
        list = newList;
        --count;
        return true;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    // Полная очистка таблицы
    void clear() {
        for (int i = 0; i < capacity; ++i) {
            buckets.GetRef(i).Clear();
        }
        count = 0;
    }
};

#endif