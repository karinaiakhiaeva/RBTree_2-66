#include "BST.h"
#include <iostream>
#include <stack>
#include <functional>
#include <algorithm>
#include <stdexcept>

// ==================== Static Helper Methods ====================

BinarySearchTree::Node* BinarySearchTree::getLeftmost(Node *node) {
    if (!node) return nullptr;
    while (node->left) node = node->left;
    return node;
}

BinarySearchTree::Node* BinarySearchTree::getRightmost(Node *node) {
    if (!node) return nullptr;
    while (node->right) node = node->right;
    return node;
}

const BinarySearchTree::Node* BinarySearchTree::getLeftmost(const Node *node) {
    if (!node) return nullptr;
    while (node->left) node = node->left;
    return node;
}

const BinarySearchTree::Node* BinarySearchTree::getRightmost(const Node *node) {
    if (!node) return nullptr;
    while (node->right) node = node->right;
    return node;
}

// ==================== RB-Tree Operations ====================

void BinarySearchTree::rotateLeft(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) _root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void BinarySearchTree::rotateRight(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) _root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->right = x;
    x->parent = y;
}

void BinarySearchTree::insertFixup(Node *z) {
    while (z->parent && !z->parent->color) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y && !y->color) {
                z->parent->color = true;
                y->color = true;
                z->parent->parent->color = false;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = true;
                z->parent->parent->color = false;
                rotateRight(z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y && !y->color) {
                z->parent->color = true;
                y->color = true;
                z->parent->parent->color = false;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = true;
                z->parent->parent->color = false;
                rotateLeft(z->parent->parent);
            }
        }
    }
    _root->color = true;
}

void BinarySearchTree::transplant(Node *u, Node *v) {
    if (!u->parent) _root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

void BinarySearchTree::deleteFixup(Node *x, Node *xParent) {
    while (x != _root && (!x || x->color)) {
        if (x == xParent->left) {
            Node *w = xParent->right;
            if (w && !w->color) {
                w->color = true;
                xParent->color = false;
                rotateLeft(xParent);
                w = xParent->right;
            }
            if (!w || ((!w->left || w->left->color) && (!w->right || w->right->color))) {
                if (w) w->color = false;
                x = xParent;
                xParent = x->parent;
            } else {
                if (!w->right || w->right->color) {
                    if (w->left) w->left->color = true;
                    w->color = false;
                    rotateRight(w);
                    w = xParent->right;
                }
                w->color = xParent->color;
                xParent->color = true;
                if (w->right) w->right->color = true;
                rotateLeft(xParent);
                x = _root;
            }
        } else {
            Node *w = xParent->left;
            if (w && !w->color) {
                w->color = true;
                xParent->color = false;
                rotateRight(xParent);
                w = xParent->left;
            }
            if (!w || ((!w->right || w->right->color) && (!w->left || w->left->color))) {
                if (w) w->color = false;
                x = xParent;
                xParent = x->parent;
            } else {
                if (!w->left || w->left->color) {
                    if (w->right) w->right->color = true;
                    w->color = false;
                    rotateLeft(w);
                    w = xParent->left;
                }
                w->color = xParent->color;
                xParent->color = true;
                if (w->left) w->left->color = true;
                rotateRight(xParent);
                x = _root;
            }
        }
    }
    if (x) x->color = true;
}

void BinarySearchTree::clearSubtree(Node *node) {
    if (!node) return;
    clearSubtree(node->left);
    clearSubtree(node->right);
    delete node;
}

void BinarySearchTree::outputSubtree(Node *node) const {
    if (!node) return;
    outputSubtree(node->left);
    std::cout << "(" << node->keyValuePair.first << ", " << node->keyValuePair.second << ") ";
    outputSubtree(node->right);
}

// ==================== Search Methods ====================

BinarySearchTree::Iterator BinarySearchTree::lower_bound(const Key &key) {
    Node *current = _root;
    Node *result = nullptr;
    while (current) {
        if (current->keyValuePair.first >= key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return Iterator(result);
}

BinarySearchTree::ConstIterator BinarySearchTree::lower_bound(const Key &key) const {
    const Node *current = _root;
    const Node *result = nullptr;
    while (current) {
        if (current->keyValuePair.first >= key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return ConstIterator(result);
}

BinarySearchTree::Iterator BinarySearchTree::upper_bound(const Key &key) {
    Node *current = _root;
    Node *result = nullptr;
    while (current) {
        if (current->keyValuePair.first > key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return Iterator(result);
}

BinarySearchTree::ConstIterator BinarySearchTree::upper_bound(const Key &key) const {
    const Node *current = _root;
    const Node *result = nullptr;
    while (current) {
        if (current->keyValuePair.first > key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return ConstIterator(result);
}

// ==================== Node Implementation ====================

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
    : keyValuePair{key, value}, parent(parent), left(left), right(right), color(false) {}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr), color(other.color) {}

bool BinarySearchTree::Node::operator==(const Node &other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) left->output_node_tree();
    std::cout << "(" << keyValuePair.first << ", " << keyValuePair.second << ") ";
    if (right) right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key <= keyValuePair.first) {
        if (left) left->insert(key, value);
        else left = new Node(key, value, this);
    } else {
        if (right) right->insert(key, value);
        else right = new Node(key, value, this);
    }
}

void BinarySearchTree::Node::erase(const Key &key) {
    if (key < keyValuePair.first && left) {
        left->erase(key);
    } else if (key > keyValuePair.first && right) {
        right->erase(key);
    } else if (key == keyValuePair.first) {
        if (!left && !right) {
            if (parent) {
                if (parent->left == this) parent->left = nullptr;
                else parent->right = nullptr;
            }
            delete this;
        } else if (!left && right) {
            if (parent) {
                if (parent->left == this) parent->left = right;
                else parent->right = right;
            }
            right->parent = parent;
            delete this;
        } else if (left && !right) {
            if (parent) {
                if (parent->left == this) parent->left = left;
                else parent->right = left;
            }
            left->parent = parent;
            delete this;
        } else {
            Node *successor = right;
            while (successor->left) successor = successor->left;
            std::pair<Key, Value> temp = successor->keyValuePair;
            keyValuePair = temp;
            successor->erase(temp.first);
        }
    }
}

// ==================== BinarySearchTree Implementation ====================

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(0), _root(nullptr) {
    if (!other._root) return;
    
    std::stack<std::pair<Node*, const Node*>> stack;
    _root = new Node(*other._root);
    _size++;
    stack.push({_root, other._root});
    
    while (!stack.empty()) {
        auto [myNode, otherNode] = stack.top();
        stack.pop();
        
        if (otherNode->left) {
            myNode->left = new Node(*otherNode->left);
            myNode->left->parent = myNode;
            _size++;
            stack.push({myNode->left, otherNode->left});
        }
        
        if (otherNode->right) {
            myNode->right = new Node(*otherNode->right);
            myNode->right->parent = myNode;
            _size++;
            stack.push({myNode->right, otherNode->right});
        }
    }
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        BinarySearchTree temp(other);
        std::swap(_root, temp._root);
        std::swap(_size, temp._size);
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
    : _size(other._size), _root(other._root) {
    other._size = 0;
    other._root = nullptr;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this != &other) {
        clearSubtree(_root);
        _root = other._root;
        _size = other._size;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    clearSubtree(_root);
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    Node *z = new Node(key, value);
    Node *y = nullptr;
    Node *x = _root;
    
    while (x) {
        y = x;
        if (key <= x->keyValuePair.first) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    z->parent = y;
    if (!y) {
        _root = z;
    } else if (key <= y->keyValuePair.first) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    z->color = false;
    _size++;
    insertFixup(z);
}

void BinarySearchTree::erase(const Key &key) {
    bool deleted = true;
    while (deleted) {
        deleted = false;
        Node *z = _root;
        
        while (z) {
            if (key < z->keyValuePair.first) {
                z = z->left;
            } else if (key > z->keyValuePair.first) {
                z = z->right;
            } else {
                deleted = true;
                break;
            }
        }
        
        if (!z) break;
        
        Node *y = z;
        Node *x = nullptr;
        Node *xParent = nullptr;
        bool yOriginalColor = y->color;
        
        if (!z->left) {
            x = z->right;
            xParent = z->parent;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            xParent = z->parent;
            transplant(z, z->left);
        } else {
            y = getLeftmost(z->right);
            yOriginalColor = y->color;
            x = y->right;
            
            if (y->parent == z) {
                if (x) x->parent = y;
                xParent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
                xParent = y->parent;
            }
            
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        _size--;
        
        if (yOriginalColor) {
            deleteFixup(x, xParent);
        }
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return ConstIterator(current);
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return Iterator(current);
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    return std::make_pair(lower_bound(key), upper_bound(key));
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    return std::make_pair(lower_bound(key), upper_bound(key));
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    const Node *node = getLeftmost(_root);
    if (!node) return cend();
    return ConstIterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    const Node *node = getRightmost(_root);
    if (!node) return cend();
    return ConstIterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    Node *current = _root;
    Node *result = nullptr;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            result = current;
            current = current->left;
        }
    }
    if (!result) return cend();
    return ConstIterator(result);
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    Node *current = _root;
    Node *result = nullptr;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            result = current;
            current = current->right;
        }
    }
    if (!result) return cend();
    return ConstIterator(result);
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node *node = getLeftmost(_root);
    if (!node) return end();
    return Iterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    const Node *node = getLeftmost(_root);
    if (!node) return cend();
    return ConstIterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    outputSubtree(_root);
    std::cout << std::endl;
}

size_t BinarySearchTree::max_height() const {
    if (!_root) return 0;
    std::function<size_t(Node*)> height = [&](Node *node) -> size_t {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    };
    return height(_root);
}

// ==================== Iterator Implementation ====================

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return &(_node->keyValuePair);
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return &(_node->keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) return *this;
    
    if (_node->right) {
        _node = BinarySearchTree::getLeftmost(_node->right);
    } else {
        Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node) return *this;
    
    if (_node->left) {
        _node = BinarySearchTree::getRightmost(_node->left);
    } else {
        Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node != other._node;
}

// ==================== ConstIterator Implementation ====================

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) throw std::runtime_error("Dereferencing null const iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) throw std::runtime_error("Dereferencing null const iterator");
    return &(_node->keyValuePair);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node) return *this;
    
    if (_node->right) {
        _node = BinarySearchTree::getLeftmost(_node->right);
    } else {
        const Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) return *this;
    
    if (_node->left) {
        _node = BinarySearchTree::getRightmost(_node->left);
    } else {
        const Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}
