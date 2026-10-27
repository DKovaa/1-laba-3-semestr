#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdexcept>
#include <string>
#include <sstream>

template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // --- Вспомогательные рекурсивные функции ---

    void insertRecursive(Node*& node, const T& value) {
        if (node == nullptr) {
            node = new Node(value);
            return;
        }
        if (value < node->data) insertRecursive(node->left, value);
        else if (value > node->data) insertRecursive(node->right, value);
    }

    Node* findRecursive(Node* node, const T& value) const {
        if (node == nullptr) return nullptr;
        if (value == node->data) return node;
        if (value < node->data) return findRecursive(node->left, value);
        return findRecursive(node->right, value);
    }

    void clearRecursive(Node* node) {
        if (node != nullptr) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }

    Node* copyTree(Node* node) const {
        if (node == nullptr) return nullptr;
        Node* newNode = new Node(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    bool isSubtreeRecursive(Node* mainRoot, Node* subRoot) const {
        if (subRoot == nullptr) return true;
        if (mainRoot == nullptr) return false;
        if (mainRoot->data == subRoot->data) {
            return isSubtreeRecursive(mainRoot->left, subRoot->left) &&
                   isSubtreeRecursive(mainRoot->right, subRoot->right);
        }
        return isSubtreeRecursive(mainRoot->left, subRoot) ||
               isSubtreeRecursive(mainRoot->right, subRoot);
    }

    // --- 6 обходов ---
    
    void inorderRecursive(Node* node, void (*func)(const T&)) const { // КЛП
        if (node) {
            inorderRecursive(node->left, func);
            func(node->data);
            inorderRecursive(node->right, func);
        }
    }

    void preorderRecursive(Node* node, void (*func)(const T&)) const { // КПЛ
        if (node) {
            func(node->data);
            preorderRecursive(node->left, func);
            preorderRecursive(node->right, func);
        }
    }

    void postorderRecursive(Node* node, void (*func)(const T&)) const { // ЛПК
        if (node) {
            postorderRecursive(node->left, func);
            postorderRecursive(node->right, func);
            func(node->data);
        }
    }

    void reverseInorderRecursive(Node* node, void (*func)(const T&)) const { // ПКЛ
        if (node) {
            reverseInorderRecursive(node->right, func);
            func(node->data);
            reverseInorderRecursive(node->left, func);
        }
    }

    void reversePreorderRecursive(Node* node, void (*func)(const T&)) const { // КПЛ (обратный порядок детей)
        if (node) {
            func(node->data);
            reversePreorderRecursive(node->right, func);
            reversePreorderRecursive(node->left, func);
        }
    }

    void reversePostorderRecursive(Node* node, void (*func)(const T&)) const { // ПЛК
        if (node) {
            reversePostorderRecursive(node->right, func);
            reversePostorderRecursive(node->left, func);
            func(node->data);
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        clear();
    }

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    // --- Базовые операции ---

    void insert(const T& value) {
        insertRecursive(root, value);
    }

    bool contains(const T& value) const {
        return findRecursive(root, value) != nullptr;
    }

    void remove(const T& value) {
        throw std::logic_error("Remove is not implemented in this minimal version");
    }

    void clear() {
        clearRecursive(root);
        root = nullptr;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    // --- 6 обходов (публичные методы) ---

    void traverseKLP(void (*func)(const T&)) const { inorderRecursive(root, func); }
    void traverseKPL(void (*func)(const T&)) const { reversePreorderRecursive(root, func); }
    void traverseLPK(void (*func)(const T&)) const { postorderRecursive(root, func); }
    void traverseLKP(void (*func)(const T&)) const { inorderRecursive(root, func); }
    void traversePLK(void (*func)(const T&)) const { reversePostorderRecursive(root, func); }
    void traversePKL(void (*func)(const T&)) const { reverseInorderRecursive(root, func); }

    // --- Операции высшего порядка (map, where, reduce) ---

    template <typename U>
    BinaryTree<U> map(U (*func)(const T&)) const {
        BinaryTree<U> newTree;
        struct Helper {
            static void traverse(Node* n, BinaryTree<U>& t, U (*f)(const T&)) {
                if (n) {
                    traverse(n->left, t, f);
                    t.insert(f(n->data));
                    traverse(n->right, t, f);
                }
            }
        };
        Helper::traverse(root, newTree, func);
        return newTree;
    }

    BinaryTree<T> where(bool (*predicate)(const T&)) const {
        BinaryTree<T> newTree;
        struct Helper {
            static void traverse(Node* n, BinaryTree<T>& t, bool (*p)(const T&)) {
                if (n) {
                    traverse(n->left, t, p);
                    if (p(n->data)) t.insert(n->data);
                    traverse(n->right, t, p);
                }
            }
        };
        Helper::traverse(root, newTree, predicate);
        return newTree;
    }

    template <typename U>
    U reduce(U (*func)(const U&, const T&), U initial) const {
        U result = initial;
        struct Helper {
            static void traverse(Node* n, U& res, U (*f)(const U&, const T&)) {
                if (n) {
                    traverse(n->left, res, f);
                    res = f(res, n->data);
                    traverse(n->right, res, f);
                }
            }
        };
        Helper::traverse(root, result, func);
        return result;
    }

    // --- Сохранение в строку  ---

    std::string toString() const {
        std::ostringstream oss;
        bool first = true;
        struct Helper {
            static void traverse(Node* n, std::ostringstream& o, bool& f) {
                if (n) {
                    traverse(n->left, o, f);
                    if (!f) o << " ";
                    o << n->data;
                    f = false;
                    traverse(n->right, o, f);
                }
            }
        };
        Helper::traverse(root, oss, first);
        return oss.str();
    }

    // --- Извлечение поддерева ---

    BinaryTree<T> extractSubtree(const T& rootValue) const {
        Node* subRoot = findRecursive(root, rootValue);
        if (subRoot == nullptr) throw std::out_of_range("Node not found for subtree extraction");
        BinaryTree<T> newTree;
        newTree.root = copyTree(subRoot);
        return newTree;
    }

    // --- Поиск на вхождение поддерева ---

    bool containsSubtree(const BinaryTree<T>& other) const {
        if (other.isEmpty()) return true;
        return isSubtreeRecursive(root, other.root);
    }
};

#endif