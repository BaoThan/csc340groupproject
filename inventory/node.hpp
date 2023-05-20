#ifndef NODE_HPP
#define NODE_HPP

#include "../product/product.hpp"

class Node
{
public:
    Product product;
    Node *prev;
    Node *next;

    Node(Product newProduct) : product(newProduct), prev(nullptr), next(nullptr) {}

    void print()
    {
        product.print();
    }

    void setNext(Node *newNode)
    {
        next = newNode;
    }

    Node *getNext()
    {
        return next;
    }

    void setPrev(Node *newNode)
    {
        prev = newNode;
    }

    Node *getPrev()
    {
        return prev;
    }
};

#endif
