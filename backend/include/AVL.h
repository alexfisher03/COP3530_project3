#pragma once
#include "Car.h"
#include <memory>

// AVL Tree Node 
struct AVLNode {
    Car car;
    std::shared_ptr<AVLNode> left;
    std::shared_ptr<AVLNode> right;
    int height;

    AVLNode(const Car& car);
};

class AVLTree {
public:
    AVLTree();
    void insert(const Car& car);
    void printInOrder() const;

private:
    std::shared_ptr<AVLNode> root;

    std::shared_ptr<AVLNode> insert(std::shared_ptr<AVLNode> node, const Car& car);
    int height(std::shared_ptr<AVLNode> node) const;
    int getBalance(std::shared_ptr<AVLNode> node) const;
    std::shared_ptr<AVLNode> rightRotate(std::shared_ptr<AVLNode> y);
    std::shared_ptr<AVLNode> leftRotate(std::shared_ptr<AVLNode> x);
    void printInOrder(std::shared_ptr<AVLNode> node) const;
};

