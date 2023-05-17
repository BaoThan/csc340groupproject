#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>
#include <cctype>
#include <ctime>
#include <chrono>
#include <vector>
using namespace std;

class Product
{
public:
    string name;
    double price;
    int quantity;
    string category;
    string expDate;

    Product()
    {
        name = "";
        price = 0;
        quantity = 0;
        category = "";
        expDate = "N/A";
    }

    Product(string name, double price, int quantity, string category, string expDate)
    {
        this->name = name;
        this->price = price;
        this->quantity = quantity;
        this->category = category;
        this->expDate = expDate;
    }

    bool isExpired()
    {
        if (expDate == "N/A")
            return false;

        // Get the current date
        time_t t = time(0);
        tm *now = localtime(&t);
        int currentYear = now->tm_year + 1900;
        int currentMonth = now->tm_mon + 1;
        int currentDay = now->tm_mday;

        // Parse the expiration date
        int expYear, expMonth, expDay;
        sscanf(expDate.c_str(), "%d-%d-%d", &expYear, &expMonth, &expDay);

        // Compare the dates
        if (expYear < currentYear)
            return true;
        else if (expYear == currentYear && expMonth < currentMonth)
            return true;
        else if (expYear == currentYear && expMonth == currentMonth && expDay < currentDay)
            return true;
        else
            return false;
    }

    void print() const
    {
        cout << left << setw(30) << name;
        cout << right << setw(5) << fixed << setprecision(2) << "$ " << left << setw(15) << price;
        cout << left << setw(10) << quantity;
        cout << right << setw(10) << category << endl;
        cout << right << setw(15) << expDate << endl;
    }
};

class Node
{
public:
    Node *next;
    Node *prev;
    Product product;

    Node(Product newProduct)
    {
        product = newProduct;
        prev = nullptr;
        next = nullptr;
    }

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

class Inventory
{
private:
    Node *head;
    Node *tail;

public:
    Inventory()
    {
        head = nullptr;
        tail = nullptr;
    }

    Node *getHead()
    {
        return head;
    }

    void writeInventoryToFile(const string &filename)
    {
        ofstream outFile(filename);
        if (!outFile)
        {
            cout << "Unable to open file " << filename << "\n";
            return;
        }

        // Write the header line
        outFile << "Name,Price,Quantity,Category,Expiration Date" << endl;

        // Write each product to the file
        Node *current = head;
        while (current != nullptr)
        {
            Product product = current->product;
            outFile << product.name << ","
                    << product.price << ","
                    << product.quantity << ","
                    << product.category << ","
                    << product.expDate << endl;
            current = current->getNext();
        }

        outFile.close();
        cout << "Inventory written to file: " << filename << endl;
    }

    void shuffleProducts()
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

    void addProduct(string name, double price, int quantity, string category, string expDate)
    {
        Product newProduct = Product(name, price, quantity, category, expDate);
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

    void displayProducts()
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

    void deleteProduct(Node *deleteNode)
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

    Node *searchProduct(string name)
    {
        if (head == nullptr)
            return nullptr;

        Node *current = head;
        while (current->getNext() != nullptr && current->product.name != name)
        {
            current = current->getNext();
        }
        if (current->product.name == name)
            return current;
        else
            return nullptr;
    }

    void updatePrice(Node *changeNode, double newPrice)
    {
        changeNode->product.price = newPrice;
    }

    void updateQuantity(Node *changeNode, int newQuantity)
    {
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
    cout << "7. Display categories\n";
    cout << "8. Shuffle the inventory\n";
    cout << "9. Write inventory to file\n";
    cout << "10. Filter expired Products and export to file\n";
    cout << "0. Quit\n";
}

void printCategoryMenu()
{
    cout << "Categories:\n";
    cout << " 0. <  Go back to previous menu\n";
    cout << " 1. Fruit" << setw(30) << " 8. Beverage\n";
    cout << " 2. Car Accessories" << setw(19) << " 9. Fitness\n";
    cout << " 3. Electronics" << setw(22) << "10. Shoes\n";
    cout << " 4. Sports" << setw(36) << "11. Home & Kitchen\n";
    cout << " 5. Accessories" << setw(25) << "12. Clothing\n";
    cout << " 6. Outdoor" << setw(27) << "13. Travel\n";
    cout << " 7. Books" << setw(36) << "14. Personal Care\n";
}

void displayProductsByCategory(const int categoryChoice, Inventory &inventory)
{
    string category;
    switch (categoryChoice)
    {
    case 1:
        category = "Fruit";
        break;
    case 2:
        category = "Car Accessories";
        break;
    case 3:
        category = "Electronics";
        break;
    case 4:
        category = "Sports";
        break;
    case 5:
        category = "Accessories";
        break;
    case 6:
        category = "Outdoor";
        break;
    case 7:
        category = "Books";
        break;
    case 8:
        category = "Beverage";
        break;
    case 9:
        category = "Fitness";
        break;
    case 10:
        category = "Shoes";
        break;
    case 11:
        category = "Home & Kitchen";
        break;
    case 12:
        category = "Clothing";
        break;
    case 13:
        category = "Travel";
        break;
    case 14:
        category = "Personal Care";
        break;
    default:
        cout << "Invalid category choice.\n";
        return;
    }

    cout << "\nDisplaying products in the category: " << category << "\n\n";
    cout << left << setw(30) << "Name" << setw(17) << "Price" << setw(15) << "Quantity"
         << "Category" << endl;
    cout << endl;

    Node *current = inventory.getHead();
    bool found = false;
    while (current != nullptr)
    {
        if (current->product.category == category)
        {
            current->print();
            found = true;
        }
        current = current->getNext();
    }

    if (!found)
        cout << "No products found in the category: " << category << endl;

    cout << endl;
}

void filterExpiredProducts(Inventory &inventory)
{
    vector<Product> expiredProducts;

    Node *current = inventory.getHead();
    while (current != nullptr)
    {
        if (current->product.isExpired())
        {
            expiredProducts.push_back(current->product);
        }
        current = current->getNext();
    }

    if (expiredProducts.empty())
    {
        cout << "No expired products found.\n";
    }
    else
    {
        cout << "Expired Products:\n";
        for (const auto &product : expiredProducts)
        {
            product.print();
        }

        cout << "Do you want to export the list to a CSV file? (Y/N): ";
        char exportChoice;
        cin >> exportChoice;

        if (tolower(exportChoice) == 'y')
        {
            string exportFilename;
            cout << "Enter the filename to export the list to (without extension): ";
            cin >> exportFilename;
            exportFilename += ".csv";

            ofstream outFile(exportFilename);
            if (!outFile)
            {
                cout << "Unable to open file " << exportFilename << "\n";
            }
            else
            {
                outFile << "Name,Price,Quantity,Category,Expiration Date" << endl;
                for (const auto &product : expiredProducts)
                {
                    outFile << product.name << ","
                            << product.price << ","
                            << product.quantity << ","
                            << product.category << ","
                            << product.expDate << endl;
                }
                outFile.close();

                cout << "List exported to file: " << exportFilename << endl;
            }
        }
        // Delete expired products from inventory
        cout << "Do you want to delete all the expired products? (Y/N): ";
        char delExpChoice;
        cin >> delExpChoice;
        if (tolower(delExpChoice) == 'y')
        {
            for (const auto &product : expiredProducts)
            {
                Node *expiredNode = inventory.searchProduct(product.name);
                if (expiredNode != nullptr)
                {
                    inventory.deleteProduct(expiredNode);
                }
            }
            cout << "All expired products deleted" << endl;
        }
    }

    cout << endl;
}

int getMenuChoice()
{
    int choice;
    cout << "Enter your choice (1-10): ";
    cin >> choice;
    cout << "\n";
    return choice;
}

void getProductDetails(string &name, double &price, int &quantity, string &category, string &expDate)
{
    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter product quantity: ";
    cin >> quantity;
    cout << "Enter product category: ";
    cin.ignore();
    getline(cin, category);
    cout << "Enter product expiration date (YYYY-MM-DD): ";
    cin >> expDate;
}

void addProduct(Inventory &inventory)
{
    string name, category, expDate;
    double price;
    int quantity;
    getProductDetails(name, price, quantity, category, expDate);
    inventory.addProduct(name, price, quantity, category, expDate);
}

void deleteProduct(Inventory &inventory)
{
    cout << "\nDeleting Product" << endl;
    string deleteName;
    cout << "Enter the name of the product to be deleted: ";
    cin.ignore();
    getline(cin, deleteName);
    Node *deleteNode = inventory.searchProduct(deleteName);
    if (deleteNode == nullptr)
        cout << "\nProduct not found. Try again." << endl;
    else
    {
        cout << "\nThe following product has been deleted." << endl;
        deleteNode->print();
        inventory.deleteProduct(deleteNode);
    }
    cout << endl;
}

void searchProduct(Inventory &inventory)
{
    string searchName;
    cout << "Enter the name of the product to be searched: ";
    cin.ignore();
    getline(cin, searchName);

    // Auto-capitalize first letter
    if (!searchName.empty())
    {
        searchName[0] = toupper(searchName[0]);
    }

    Node *searchNode = inventory.searchProduct(searchName);
    if (searchNode == nullptr)
    {
        cout << "\nProduct not found. Try again." << endl;
    }
    else
    {
        cout << "The product with the name '" << searchName << "' is found:\n";
        searchNode->print();
    }
    cout << endl;
}

void updatePrice(Inventory &inventory)
{
    string changeName;
    double newPrice;
    cout << "Enter the name of the product to update the price: ";
    cin.ignore();
    getline(cin, changeName);
    cout << "Enter the new price of that product: ";
    cin >> newPrice;
    Node *changeNode = inventory.searchProduct(changeName);
    if (changeNode == nullptr)
        cout << "\nProduct not found. Try again." << endl;
    else
    {
        inventory.updatePrice(changeNode, newPrice);
        cout << "\nThe price of the product '" << changeName << "' has been updated.\n";
        changeNode->print();
    }
    cout << endl;
}

void updateQuantity(Inventory &inventory)
{
    string changeName;
    int newQuantity;
    cout << "Enter the name of the product to update the quantity: ";
    cin.ignore();
    getline(cin, changeName);
    cout << "Enter the new quantity of that product: ";
    cin >> newQuantity;
    Node *changeNode = inventory.searchProduct(changeName);
    if (changeNode == nullptr)
        cout << "\nProduct not found. Try again." << endl;
    else
    {
        inventory.updateQuantity(changeNode, newQuantity);
        cout << "\nThe quantity of the product '" << changeName << "' has been updated.\n";
        changeNode->print();
    }
    cout << endl;
}

void readInventoryFromFile(Inventory &inventory, const string &filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cout << "Unable to open file " << filename << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string name, priceStr, quantityStr, category, expDate;
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, quantityStr, ',');
        getline(ss, category, ',');
        getline(ss, expDate, ',');

        double price = stod(priceStr);
        int quantity = stoi(quantityStr);

        inventory.addProduct(name, price, quantity, category, expDate);
    }

    inFile.close();
}

int main()
{
    cout << "Welcome to Supermarket Inventory Manager\n\n";

    Inventory inventory;
    readInventoryFromFile(inventory, "database.csv");

    int choice;
    do
    {
        printMenu();
        choice = getMenuChoice();
        string filename;

        switch (choice)
        {
        case 1:
            cout << "Displaying Inventory\n";
            inventory.displayProducts();
            cout << endl;
            break;
        case 2:
            cout << "Adding Product\n";
            addProduct(inventory);
            cout << endl;
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
        case 7:
            printCategoryMenu();
            choice = getMenuChoice();
            if (choice == 0)
            {
                break;
            }
            else
            {
                displayProductsByCategory(choice, inventory);
            }
            break;
        case 8:
            cout << "Shuffling Products\n";
            inventory.shuffleProducts();
            cout << "Products shuffled.\n";
            break;
        case 9:
            cout << "Writing Inventory to File\n";
            cout << "Enter the filename to write the inventory to: ";
            cin >> filename;
            filename += ".csv";
            inventory.writeInventoryToFile(filename);
            cout << endl;
            break;
        case 10:
            cout << "Filtering Expired Products\n";
            filterExpiredProducts(inventory);
            break;

        case 0:
            cout << "Quitting Program\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
