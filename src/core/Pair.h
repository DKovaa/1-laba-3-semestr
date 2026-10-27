#ifndef PAIR_H
#define PAIR_H

template <typename K, typename V>
class Pair {
public:
    K first;
    V second;

    Pair(const K& key, const V& value) : first(key), second(value) {}

    bool operator==(const Pair& other) const {
        return first == other.first;
    }
};

#endif