#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "node.hpp"

#include <string>

using namespace std;

class Inventory
{
private:
    Node *head;
    Node *tail;

public:
    Inventory() : head(nullptr), tail(nullptr) {}

    Node *getHead()
    {
        return head;
    }

    void writeInventoryToFile(const string &filename);

    // this here to test the sort algorithm
    void shuffleProducts();

    void addProduct(string name, double price, double cost, int quantity, string category, string expDate);

    void displayProducts();

    void deleteProduct(Node *deleteNode);

    Node *searchProduct(string name);

    void updatePrice(Node *changeNode, double newPrice);

    void updateQuantity(Node *changeNode, int newQuantity);

    void sortByCategory();

    void sortByExpirationDate();

private:
    void swapNodes(Node *node1, Node *node2);

    int compareExpirationDate(const string &date1, const string &date2);
};

#endif
