#pragma once

#include <utility>
#include <cstdint>
#include <string>

using Key = uint32_t;
using Value = double;

class BinarySearchTree 
{
    struct Node 
    {
        Node(Key key, Value value, 
             Node *parent = nullptr, Node *left = nullptr, 
             Node *right = nullptr);

        Node(const Node &other);
        bool operator==(const Node &other) const;

        void output_node_tree() const;
        void insert(const Key &key, const Value &value);
        void erase(const Key &key);

        std::pair<Key, Value> keyValuePair;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        
        bool color = false;
    };

public:
    BinarySearchTree() = default;
    explicit BinarySearchTree(const BinarySearchTree &other);
    BinarySearchTree &operator=(const BinarySearchTree &other);
    explicit BinarySearchTree(BinarySearchTree &&other) noexcept;
    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept;
    ~BinarySearchTree();

    class Iterator 
    {
    public:
        explicit Iterator(Node *node = nullptr);
        std::pair<Key, Value> &operator*();
        const std::pair<Key, Value> &operator*() const;
        std::pair<Key, Value> *operator->();
        const std::pair<Key, Value> *operator->() const;
        Iterator operator++();
        Iterator operator++(int);
        Iterator operator--();
        Iterator operator--(int);
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
    private:
        Node *_node;
        friend class BinarySearchTree;
    };

    class ConstIterator 
    {
    public:
        explicit ConstIterator(const Node *node = nullptr);
        const std::pair<Key, Value> &operator*() const;
        const std::pair<Key, Value> *operator->() const;
        ConstIterator operator++();
        ConstIterator operator++(int);
        ConstIterator operator--();
        ConstIterator operator--(int);
        bool operator==(const ConstIterator &other) const;
        bool operator!=(const ConstIterator &other) const;
    private:
        const Node *_node;
        friend class BinarySearchTree;
    };

    void insert(const Key &key, const Value &value);
    void erase(const Key &key);
    ConstIterator find(const Key &key) const;
    Iterator find(const Key &key);

    std::pair<Iterator, Iterator> equalRange(const Key &key);
    std::pair<ConstIterator, ConstIterator> equalRange(const Key &key) const;
    
    ConstIterator min() const;
    ConstIterator max() const;
    ConstIterator min(const Key &key) const;
    ConstIterator max(const Key &key) const;

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    size_t size() const;
    void output_tree();
    size_t max_height() const;

private:
    size_t _size = 0;
    Node *_root = nullptr;
    
    static Node* getLeftmost(Node *node);
    static Node* getRightmost(Node *node);
    static const Node* getLeftmost(const Node *node);
    static const Node* getRightmost(const Node *node);
    void rotateLeft(Node *x);
    void rotateRight(Node *x);
    void insertFixup(Node *z);
    void transplant(Node *u, Node *v);
    void deleteFixup(Node *x, Node *xParent);
    void clearSubtree(Node *node);
    void outputSubtree(Node *node) const;
    
    Iterator lower_bound(const Key &key);
    ConstIterator lower_bound(const Key &key) const;
    Iterator upper_bound(const Key &key);
    ConstIterator upper_bound(const Key &key) const;
    
    Node* findNode(const Key &key) const;
    Node* findFirst(const Key &key) const;
    Node* findLast(const Key &key) const;
};
