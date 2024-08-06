#include "AVL.h"
#include <iostream> 
#include <algorithm>
#include <unordered_set>

AVLNode::AVLNode(const Car& car) : car(car), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr) {}

void AVLTree::insert(const Car& car) {
    root = insert(root, car);
}

std::shared_ptr<AVLNode> AVLTree::insert(std::shared_ptr<AVLNode> node, const Car& car) {
    if (!node)
        return std::make_shared<AVLNode>(car);

    if (car.model < node->car.model)
        node->left = insert(node->left, car);
    else if (car.model > node->car.model)
        node->right = insert(node->right, car);
    else
        return node;

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && car.model < node->left->car.model)
        return rightRotate(node);
    if (balance < -1 && car.model > node->right->car.model)
        return leftRotate(node);
    if (balance > 1 && car.model > node->left->car.model) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && car.model < node->right->car.model) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int AVLTree::height(std::shared_ptr<AVLNode> node) const {
    return node ? node->height : 0;
}

int AVLTree::getBalance(std::shared_ptr<AVLNode> node) const {
    return node ? height(node->left) - height(node->right) : 0;
}

std::shared_ptr<AVLNode> AVLTree::rightRotate(std::shared_ptr<AVLNode> y) {
    std::shared_ptr<AVLNode> x = y->left;
    std::shared_ptr<AVLNode> T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

std::shared_ptr<AVLNode> AVLTree::leftRotate(std::shared_ptr<AVLNode> x) {
    std::shared_ptr<AVLNode> y = x->right;
    std::shared_ptr<AVLNode> T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

void AVLTree::printUniqueModels() const {
    std::unordered_set<std::string> models;
    printInOrder(root, models);
}

void AVLTree::printInOrder(std::shared_ptr<AVLNode> node, std::unordered_set<std::string>& models) const {
    if (node) {
        printInOrder(node->left, models);
        if (models.find(node->car.model) == models.end()) {
            std::cout << "\n|" << node->car.model << " | ";
            models.insert(node->car.model);
        }
        printInOrder(node->right, models);
    }
}

std::vector<Car> AVLTree::searchByModel(const std::string& model) const {
    std::vector<Car> cars;
    searchInOrder(root, model, cars);
    return cars;
}

void AVLTree::searchInOrder(std::shared_ptr<AVLNode> node, const std::string& model, std::vector<Car>& cars) const {
    if (node) {
        searchInOrder(node->left, model, cars);
        if (node->car.model == model) {
            cars.push_back(node->car);
        }
        searchInOrder(node->right, model, cars);
    }
}