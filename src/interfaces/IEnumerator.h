#ifndef IENUMERATOR_H
#define IENUMERATOR_H

template <class T>
class IEnumerator {
public:
    virtual ~IEnumerator() {}
    virtual bool MoveNext() = 0;
    virtual const T& GetCurrent() const = 0;
};

template <class T>
class IEnumerable {
public:
    virtual ~IEnumerable() {}
    virtual IEnumerator<T>* GetEnumerator() const = 0;
};

#endif