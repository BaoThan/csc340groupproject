#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

class Product {
public:
    string name;
    int id;
    double price;
    int quantity;
    Product() {
        name = "";
        id = 0000;
        price = 0;

    }
    Product(int id, string name, double price, int quantity) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->quantity = quantity;
    }

    void print()
    {
        cout << "( " << left << setw(2) << id << " )\t";
        cout << left << setw(30) << name << "\t";
        cout << right << setw(5) << fixed << setprecision(2) << "$" << price << "\t";
        cout << right << setw(8) << quantity << endl;
    }
    string getName() {
        return (*this).name;
    }

    int getID() {
        return (*this).id;
    }
};

class Node {
 public:
    Node* next;
    Node* prev;
    Product product;

    Node(Product newProduct) {
        product = newProduct;
        prev = nullptr;
        next = nullptr;
    }
    void print() {
        product.print();
    }
    void setNext(Node* newNode) {
        (*this).next = newNode;
    }
    Node* getNext() {
        return (*this).next;
    }
    void setPrev(Node* newNode) {
        (*this).prev = newNode;
    }
    Node* getPrev() {
        return (*this).prev;
    }
    string getName() {
        return (*this).product.getName();
    }

    int getID() {
        return (*this).product.getID();
    }
    
};

class Inventory {
private:
    Node* head;
    Node* tail;

public:
    Inventory() {
        head = nullptr;
        tail = nullptr;
    }

    void addProduct(int id, string name, double price, int quantity) {
        Product newProduct = Product(id, name, price,quantity);
        Node* newNode = new Node(newProduct);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }

        else {
            tail->setNext(newNode);
            newNode->setPrev(tail);
            tail = newNode;
        }
    }

    void displayProducts() {
        cout << left << setw(8) << "ID" << setw(35) << "Name" << setw(16) << "Price" << "Quantity" << endl;
        cout << endl;

        if (head == nullptr) cout << "empty";
        else {
            Node* current = head;
            while (current != nullptr) {
                current->print();
                current = current->getNext();
            }
        }
    }

    void deleteProduct(Node* deleteNode) {
        if (head == nullptr) cout << "Inventory is empty";
        else {
            Node* current = head;
            while (current != deleteNode) {
                current = current->getNext();
            }

            if (current == nullptr) cout << "Product not found" <<endl;

            else {
                if (current == head) {
                    head = head->getNext();
                }
                else if (current == tail) {
                    tail = current->getPrev();
                    tail->setNext(nullptr);
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                delete current;
            }
        }
    }

    Node* searchProduct(int id) {
        if (head == nullptr) return nullptr;

        Node* current = head;
        while (current->next != nullptr && current->getID() != id) {

            current = current->next;
        }
        if (current->getID() == id) return current;

        else {
            return nullptr;
        }
    }

    void updatePrice(Node* changeNode, double newPrice) {
        changeNode->product.price = newPrice;
    }

    void updateQuantity(Node* changeNode, double newQuantity) {
        changeNode->product.quantity = newQuantity;
    }

    
};
void printMenu()
{
    cout << "Welcome to Supermarket Inventory Manager\n\n";
    cout << "1. Display inventory\n";
    cout << "2. Add product\n";
    cout << "3. Remove product\n";
    cout << "4. Update price\n";
    cout << "5. Update quantity\n";
    cout << "6. Search product\n";
    cout << "0. Quit\n";
}

int getMenuChoice()
{
    int choice;
    cout << "Enter your choice (1-6): ";
    cin >> choice;
    cout << "\n";
    return choice;
}

void getProductDetails(string& name, int& id, double& price, int& quantity)
{
    cout << "Enter product name: ";
    cin >> name;
    cout << "Enter product ID: ";
    cin >> id;
    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter product quantity: ";
    cin >> quantity;
}
//Calling from outside
void addProduct(Inventory& inventory) {
    string name;
    int id, quantity;
    double price;
    getProductDetails(name, id, price, quantity);
    inventory.addProduct(id, name, price, quantity);
}

void deleteProduct(Inventory& inventory) {
    cout << "\nDeleting Product" << endl;
    int deleteID;
    cout << "Enter the ID of the product to be deleted:";
    cin >> deleteID;

    Node* deleteNode = inventory.searchProduct(deleteID);
    if (deleteNode == nullptr) cout << "\nProduct not found. Try again.";
    else {
        cout << "\nThe following product has been deleted."<< endl;
        deleteNode->print();
        
        inventory.deleteProduct(deleteNode);
    }
    cout << endl;
}           

void searchProduct(Inventory& inventory) {
    int searchId;
    cout << "Enter the ID of the product to be searched: ";
    cin >> searchId;

    Node* searchNode = inventory.searchProduct(searchId);
    if (searchNode == nullptr) cout << "\nProduct not found. Try again."<<endl;
    else {
        cout << "\The Product with the ID: " << searchId << " is- " << searchNode->getName() << endl;
        searchNode->print();
    }
    cout << endl;
}

void updatePrice(Inventory& inventory) {
    int changeID;
    double newPrice;

    cout << "Enter the ID of the product to update the price: ";
    cin >> changeID;

    cout << "Enter the new price of that product: ";
    cin >> newPrice;

    Node* changeNode = inventory.searchProduct(changeID);
    if (changeNode == nullptr) cout << "\nProduct not found. Try again." << endl;
    else {
        inventory.updatePrice(changeNode, newPrice);
    }
}

void updateQuantity(Inventory& inventory) {
    int changeID;
    double newQuantity;

    cout << "Enter the ID of the product to update the quantity: ";
    cin >> changeID;

    cout << "Enter the new quantity of that product: ";
    cin >> newQuantity;

    Node* changeNode = inventory.searchProduct(changeID);
    if (changeNode == nullptr) cout << "\nProduct not found. Try again." << endl;
    else {
        inventory.updateQuantity(changeNode, newQuantity);
    }
}

int main()
{
    Inventory inventory;
    string name;
    int id;
    double price;
    int quantity;
    
    ifstream inFile;
    inFile.open("TextFile1.txt");
    if (!inFile)
    {
        cout << "Unable to open file TextFile1.txt\n";
    }
    else
    {
        string name;
        int id, quantity;
        double price;
        string line;

        // read each line of the CSV file
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;

            // read each token separated by commas
            getline(ss, token, ',');
            id = stoi(token);
            getline(ss, name, ',');
            getline(ss, token, ',');
            price = stod(token);
            getline(ss, token, ',');
            quantity = stoi(token);

            inventory.addProduct(id, name, price, quantity);
        }
        inFile.close();
    }

    int choice;
    do
    {
        printMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            inventory.displayProducts();
            break;
        case 2:
            addProduct(inventory);
            break;
        case 3:
            deleteProduct(inventory);
            break;
        case 4:
            updatePrice(inventory);
            break;
        case 5:
            updateQuantity(inventory);
            break;
        case 6:
            searchProduct(inventory);
            break;

        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 0);

    return 0;
    

    


}

