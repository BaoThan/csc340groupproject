#include "inventory.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

void Inventory::writeInventoryToFile(const string &filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cout << "Unable to open file " << filename << "\n";
        return;
    }

    // Write the header line
    outFile << "Name,Price,Cost,Quantity,Category,Expiration Date" << endl;

    // Write each product to the file
    Node *current = head;
    while (current != nullptr)
    {
        Product product = current->product;
        outFile << product.name() << ","
                << product.price() << ","
                << product.cost() << ","
                << product.quantity() << ","
                << product.category() << ","
                << product.expDate() << endl;
        current = current->getNext();
    }

    outFile.close();
    cout << "Inventory written to file: " << filename << endl;
}

void Inventory::shuffleProducts()
{
    if (head == nullptr || head == tail)
        return;

    // Collect all the products in a vector
    vector<Product> products;
    Node *current = head;
    while (current != nullptr)
    {
        products.push_back(current->product);
        current = current->getNext();
    }

    // Shuffle the products using random shuffle algorithm
    random_device rd;
    mt19937 gen(rd());
    shuffle(products.begin(), products.end(), gen);

    // Reconstruct the linked list with shuffled products
    current = head;
    for (const auto &product : products)
    {
        current->product = product;
        current = current->getNext();
    }
}

void Inventory::addProduct(string name,
                           double price,
                           double cost,
                           int quantity,
                           string category,
                           string expDate)
{
    Product newProduct = Product(name, price, cost, quantity, category, expDate);
    Node *newNode = new Node(newProduct);

    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->setNext(newNode);
        newNode->setPrev(tail);
        tail = newNode;
    }
}


void Inventory::displayProducts()
{
    cout << endl;
    cout << left << setw(33) << "Name" << setw(17) << "Price" << setw(15) << "Quantity"
            << "Category" << endl;
    cout << endl;

    if (head == nullptr)
        cout << "Inventory is empty";
    else
    {
        Node *current = head;
        while (current != nullptr)
        {
            current->print();
            current = current->getNext();
        }
    }
}

void Inventory::deleteProduct(Node *deleteNode)
{
    if (head == nullptr)
        cout << "Inventory is empty";
    else
    {
        Node *current = head;
        while (current != deleteNode)
        {
            current = current->getNext();
        }

        if (current == nullptr)
            cout << "Product not found" << endl;
        else
        {
            if (current == head)
            {
                head = head->getNext();
            }
            else if (current == tail)
            {
                tail = current->getPrev();
                tail->setNext(nullptr);
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
    }
}

Node* Inventory::searchProduct(string name)
{
    if (head == nullptr)
        return nullptr;

    Node *current = head;
    while (current->getNext() != nullptr && current->product.name() != name)
    {
        current = current->getNext();
    }
    if (current->product.name() == name)
        return current;
    else
        return nullptr;
}

void Inventory::updatePrice(Node *changeNode, double newPrice)
{
    changeNode->product.setPrice(newPrice);
}

void Inventory::updateQuantity(Node *changeNode, int newQuantity)
{
    changeNode->product.setQuantity(newQuantity);
}

void Inventory::sortByCategory()
{
    if (head == nullptr || head->next == nullptr)
    {
        return; // Nothing to sort if the inventory is empty or has only one node
    }

    bool sorted;
    Node *current;
    Node *last = nullptr;

    do
    {
        sorted = true;
        current = head;

        while (current->next != last)
        {
            if (current->product.category() > current->next->product.category())
            {
                swapNodes(current, current->next);
                sorted = false;
            }

            current = current->next;
        }

        last = current;
    } while (!sorted);
}

void Inventory::sortByExpirationDate()
{
    if (head == nullptr || head->next == nullptr)
    {
        return; // Nothing to sort if the inventory is empty or has only one node
    }

    bool sorted;
    Node *current;
    Node *last = nullptr;

    do
    {
        sorted = true;
        current = head;

        while (current->next != last)
        {
            if (current->product.expDate() != "N/A" &&
                current->next->product.expDate() != "N/A" &&
                compareExpirationDate(current->product.expDate(), current->next->product.expDate()) > 0)
            {
                swapNodes(current, current->next);
                sorted = false;
            }

            current = current->next;
        }

        last = current;
    } while (!sorted);

    // Display the sorted products with expiration date
    cout << "\nSorted Products with Expiration Date:\n";
    for (current = head; current != nullptr; current = current->getNext())
    {
        if (current->product.expDate() != "")
        {
            current->print();
        }
    }
    cout << endl;
}

void Inventory::swapNodes(Node *node1, Node *node2)
{
    Product temp = node1->product;
    node1->product = node2->product;
    node2->product = temp;
}

int Inventory::compareExpirationDate(const string &date1, const string &date2)
{
    // Assuming the date format is "YYYY-MM-DD"
    int year1, month1, day1;
    int year2, month2, day2;

    sscanf(date1.c_str(), "%d-%d-%d", &year1, &month1, &day1);
    sscanf(date2.c_str(), "%d-%d-%d", &year2, &month2, &day2);

    if (year1 != year2)
    {
        return year1 - year2;
    }
    if (month1 != month2)
    {
        return month1 - month2;
    }
    return day1 - day2;
}
