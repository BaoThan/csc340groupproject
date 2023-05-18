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
#include <unordered_set>
#include <vector>
#include <filesystem>
using namespace std;

class Product
{
public:
    string name;
    double price, cost;
    int quantity;
    string category;
    string expDate;

    Product()
    {
        name = "";
        price = 0;
        cost = 0;
        quantity = 0;
        category = "";
        expDate = "N/A";
    }
    // I just add cost to keep track of our input cost and maybe to calculate sales and profit later
    Product(string name, double price, double cost, int quantity, string category, string expDate)
    {
        this->name = name;
        this->price = price;
        this->cost = cost;
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
    // took me hour to align these items to look nice
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
        outFile << "Name,Price,Cost,Quantity,Category,Expiration Date" << endl;

        // Write each product to the file
        Node *current = head;
        while (current != nullptr)
        {
            Product product = current->product;
            outFile << product.name << ","
                    << product.price << ","
                    << product.cost << ","
                    << product.quantity << ","
                    << product.category << ","
                    << product.expDate << endl;
            current = current->getNext();
        }

        outFile.close();
        cout << "Inventory written to file: " << filename << endl;
    }
    // this here to test the sort algorithm
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

    void addProduct(string name, double price, double cost, int quantity, string category, string expDate)
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
    void sortByCategory()
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
                if (current->product.category > current->next->product.category)
                {
                    swapNodes(current, current->next);
                    sorted = false;
                }

                current = current->next;
            }

            last = current;
        } while (!sorted);
    }

    void sortByExpirationDate()
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
                if (current->product.expDate != "N/A" && current->next->product.expDate != "N/A" &&
                    compareExpirationDate(current->product.expDate, current->next->product.expDate) > 0)
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
            if (current->product.expDate != "")
            {
                current->print();
            }
        }
        cout << endl;
    }

private:
    void swapNodes(Node *node1, Node *node2)
    {
        Product temp = node1->product;
        node1->product = node2->product;
        node2->product = temp;
    }

    int compareExpirationDate(const string &date1, const string &date2)
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
};

void displayProductsByCategoryMenu(Inventory &inventory)
{
    while (true)
    {
        cout << "Categories:\n";
        cout << " 0. <  Go back to the previous menu\n";
        cout << " 1. Show All.\n";

        unordered_set<string> categories;
        Node *current = inventory.getHead();
        while (current != nullptr)
        {
            categories.insert(current->product.category);
            current = current->getNext();
        }

        int option = 2;
        vector<string> categoryOptions;
        for (const auto &category : categories)
        {
            categoryOptions.push_back(category);
            cout << " " << option << ". " << category << endl;
            option++;
        }

        int categoryChoice;
        cout << "\nEnter the category number: ";
        cin >> categoryChoice;

        if (categoryChoice == 0)
            return;

        if (categoryChoice == 1)
        {
            inventory.displayProducts();
            continue;
        }

        if (categoryChoice < 2 || categoryChoice >= option)
        {
            cout << "Invalid category choice.\n";
            continue;
        }

        string selectedCategory = categoryOptions[categoryChoice - 2];

        cout << "\nDisplaying products in the category: " << selectedCategory << "\n\n";
        cout << left << setw(30) << "Name" << setw(17) << "Price" << setw(15) << "Quantity"
             << "Category" << endl;
        cout << endl;

        current = inventory.getHead();
        bool found = false;
        while (current != nullptr)
        {
            if (current->product.category == selectedCategory)
            {
                current->print();
                found = true;
            }
            current = current->getNext();
        }

        if (!found)
            cout << "No products found in the category: " << selectedCategory << endl;

        cout << endl;
    }
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
                outFile << "Name,Price,Cost,Quantity,Category,Expiration Date" << endl;
                for (const auto &product : expiredProducts)
                {
                    outFile << product.name << ","
                            << product.price << ","
                            << product.cost << ","
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
    cout << "Enter your choice: ";
    cin >> choice;
    cout << "\n";
    return choice;
}

double getInvestmentAmount()
{
    double amount;
    cout << "Enter your investment amount in dollars: $";
    cin >> amount;
    cout << "\n";
    return amount;
}

void runSaleManager()
{
    cout << "Starting Sale Manager...\n";
    //  the Sale Manager functionality here
}
// void readInventoryFromFile(Inventory &inventory, const string &filename);
// void getProductDetails(string &name, double &price, double &cost, int &quantity, string &category, string &expDate, Inventory &inventory);
// void addProduct(Inventory &inventory);
// void deleteProduct(Inventory &inventory);
// void updatePrice(Inventory &inventory);
// void updateQuantity(Inventory &inventory);
// void searchProduct(Inventory &inventory);

void getProductDetails(string &name, double &price, double &cost, int &quantity, string &category, string &expDate, Inventory &inventory)
{
    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);

    // Capitalize first letter of each word after a space
    for (size_t i = 0; i < name.length(); i++)
    {
        if (i == 0 || name[i - 1] == ' ')
        {
            name[i] = toupper(name[i]);
        }
        else
        {
            name[i] = tolower(name[i]);
        }
    }

    // Checking for duplicate product
    Node *duplicateProduct = inventory.searchProduct(name);
    if (duplicateProduct != nullptr)
    {
        cout << "Product with the same name already exists:\n";
        duplicateProduct->print();
        cout << "Do you want to edit this product? (Y/N): ";
        char editChoice;
        cin >> editChoice;
        if (tolower(editChoice) == 'y')
        {
            cout << "Enter product price: ";
            cin >> price;
            cout << "Enter product quantity: ";
            cin >> quantity;
            cout << "Enter product category: ";
            cin.ignore();
            getline(cin, category);

            // Only ask for expiration date if the category is "Fruit" or "Beverage"
            if (category == "Fruit" || category == "Beverage")
            {
                cout << "Enter product expiration date (YYYY-MM-DD): ";
                cin >> expDate;
            }
            else
            {
                expDate = "";
            }
            return;
        }
    }

    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter product quantity: ";
    cin >> quantity;
    cout << "Enter product category: ";
    cin.ignore();
    getline(cin, category);

    // Only ask for expiration date if the category is in these, idk how to dynamicly change this so just harded code this
    if (category == "Fruit" || category == "Beverage" || category == "Milks" || category == "Eggs")
    {
        cout << "Enter product expiration date (YYYY-MM-DD): ";
        cin >> expDate;
    }
    else
    {
        expDate = "";
    }
}

void addProduct(Inventory &inventory)
{
    string name, category, expDate;
    double price, cost;
    int quantity;
    getProductDetails(name, price, cost, quantity, category, expDate, inventory);

    Node *existingProduct = inventory.searchProduct(name);
    if (existingProduct != nullptr)
    {
        // Update existing product
        existingProduct->product.price = price;
        existingProduct->product.cost = cost;
        existingProduct->product.quantity = quantity;
        existingProduct->product.category = category;
        existingProduct->product.expDate = expDate;

        cout << "The product '" << name << "' details have been updated.\n";
        existingProduct->print();
    }
    else
    {
        // Add new product
        inventory.addProduct(name, price, cost, quantity, category, expDate);
        cout << "The product '" << name << "' has been added to the inventory.\n";
    }
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

    // Auto-capitalize first letter for convenient
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
    // I think we dont need this?
    //  Get the current directory path
    filesystem::path currentPath = filesystem::current_path();

    // Go back to the parent directory (main folder)
    filesystem::path parentPath = currentPath.parent_path();

    // Combine the parent directory path with the filename
    filesystem::path filePath = parentPath / filename;

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
        string name, priceStr, costStr, quantityStr, category, expDate;
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, costStr, ',');
        getline(ss, quantityStr, ',');
        getline(ss, category, ',');
        getline(ss, expDate, ',');

        double price = stod(priceStr);
        double cost = stod(costStr);
        int quantity = stoi(quantityStr);

        inventory.addProduct(name, price, cost, quantity, category, expDate);
    }

    inFile.close();
}

void printWelcomeScreen()
{
    // Get the current date
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    string date = ctime(&currentTime);

    // Display the welcome screen with the current date
    cout << "********************************************\n";
    cout << "*       Welcome to Supermarket System       *\n";
    cout << "********************************************\n";
    cout << "Today's Date: " << date;
    cout << "********************************************\n\n";
}
void printMainMenu()
{
    cout << "Please select an option:\n";
    cout << "1. Sale Manager\n";
    cout << "2. Inventory Manager\n";
    cout << "3. Check Shipments\n";
    cout << "0. Quit\n";
}

void printMenu()
{
    cout << "Welcome to Supermarket Inventory Manager\n\n";
    cout << "1. Display inventory\n";
    cout << "2. Add product\n";
    cout << "3. Remove product\n";
    cout << "4. Update price\n";
    cout << "5. Update quantity\n";
    cout << "6. Search product\n";
    cout << "7. Sort the inventory\n";
    cout << "8. Write inventory to file\n";
    cout << "9. Filter expired Products and export to file\n";
    cout << "0. Back to Main Menu\n";
}

void printSortMenu()
{
    cout << "Sorting Options:\n";
    cout << "1. Sort inventory by category\n";
    cout << "2. Sort the inventory by expiration date\n";
    cout << "3. Shuffle inventory\n";
    cout << "0. Go back to the previous menu\n";
}
void runInventoryManager()
{
    cout << "Starting Inventory Manager...\n\n";
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
            displayProductsByCategoryMenu(inventory);
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
            printSortMenu();
            int sortChoice;
            cout << "Enter your choice: ";
            cin >> sortChoice;
            cout << endl;

            switch (sortChoice)
            {
            case 1:
                inventory.sortByCategory();
                cout << "Inventory sorted by category.\n\n";
                break;
            case 2:
                inventory.sortByExpirationDate();
                cout << "Inventory sorted by expiration date.\n\n";
                break;
            case 3:
                inventory.shuffleProducts();
                cout << "Inventory shuffled.\n\n";
                break;
            case 0:
                cout << "Going back to the previous menu.\n\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n\n";
                break;
            }
            break;
        case 8:
            cout << "Writing Inventory to File\n";
            cout << "Enter the filename to write the inventory to: ";
            cin >> filename;
            filename += ".csv";
            inventory.writeInventoryToFile(filename);
            cout << endl;
            break;
        case 9:
            cout << "Filtering Expired Products\n";
            filterExpiredProducts(inventory);
            break;
        case 0:
            cout << "Exiting Inventory Manager\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);
}
void checkShipments(double &investmentAmount, Inventory &purchasedShipments)
{
    cout << "Checking Shipments...\n";

    // Specify the directory path
    string directory = "shipments/"; // Update with your desired directory

    // Check if the directory exists
    if (!filesystem::exists(directory))
    {
        cout << "Directory does not exist: " << directory << endl;
        return;
    }

    // Create a vector to store file paths
    vector<string> fileList;

    // Iterate over files in the directory
    for (const auto &entry : filesystem::directory_iterator(directory))
    {
        // Check if the entry is a file
        if (entry.is_regular_file())
        {
            fileList.push_back(entry.path().filename().string());
        }
    }

    // Display the list of files
    if (fileList.empty())
    {
        cout << "No shipments found.\n";
        return;
    }

    cout << "List of Shipments:\n";
    int option = 1;
    for (const auto &file : fileList)
    {
        cout << option << ". " << file << endl;
        option++;
    }

    // Get the user's choice
    size_t choice;

    cout << "Enter the number of the shipment to check: ";
    cin >> choice;

    if (choice < 1 || choice > fileList.size())
    {
        cout << "Invalid choice.\n";
        return;
    }

    string selectedFile = directory + fileList[choice - 1];

    // Read the selected file and display the products
    Inventory shipmentInventory;
    readInventoryFromFile(shipmentInventory, selectedFile);

    cout << "\nProducts in shipment file '" << fileList[choice - 1] << "':\n";
    shipmentInventory.displayProducts();

    // Calculate total number of products, total quantity, and total cost
    int totalProducts = 0;
    int totalQuantity = 0;
    double totalCost = 0.0;

    Node *current = shipmentInventory.getHead();
    while (current != nullptr)
    {
        totalProducts++;
        totalQuantity += current->product.quantity;
        totalCost += current->product.cost * current->product.quantity;
        current = current->getNext();
    }

    cout << "\nTotal number of products: " << totalProducts << endl;
    cout << "Total quantity of products: " << totalQuantity << endl;
    cout << "Total cost of products: $" << totalCost << endl;

    // Compare the total cost with the investment amount
    if (totalCost <= investmentAmount)
    {
        cout << "\nDo you want to purchase the shipment? (Y/N): ";
        char purchaseChoice;
        cin >> purchaseChoice;

        if (tolower(purchaseChoice) == 'y')
        {
            // Append the purchased shipment to the existing database.csv file
            ofstream outFile("database.csv", ios::app);

            if (!outFile)
            {
                cout << "Unable to open file database.csv for appending.\n";
                return;
            }

            // Check if the file is empty
            outFile.seekp(0, ios::end);
            bool isEmpty = outFile.tellp() == 0;

            // If the file is empty, write the header first
            if (isEmpty)
            {
                outFile << "Name,Price,Cost,Quantity,Category,Expiration Date\n";
            }

            for (current = shipmentInventory.getHead(); current != nullptr; current = current->getNext())
            {
                Product product = current->product;
                outFile << product.name << ","
                        << product.price << ","
                        << product.cost << ","
                        << product.quantity << ","
                        << product.category << ","
                        << product.expDate << endl;
            }

            outFile.close();

            // Add the purchased shipment to the inventory
            purchasedShipments.addProduct(fileList[choice - 1], totalCost, totalCost, totalQuantity, "Shipment", "");

            cout << "\nCongratulations! The store has successfully purchased the shipment '" << fileList[choice - 1] << "'.\n";
            cout << "The total cost of the shipment has been deducted from the investment amount.\n";

            // Subtract the total cost of the shipment from the investment amount
            investmentAmount -= totalCost;
            cout << "Remaining investment amount: $" << investmentAmount << endl;
            // Delete the chosen file
            string filePathToDelete = directory + fileList[choice - 1];
            if (remove(filePathToDelete.c_str()) != 0)
            {
                cout << "Error deleting file: " << filePathToDelete << endl;
            }
            else
            {
                cout << "The file '" << fileList[choice - 1] << "' has been successfully deleted.\n";
            }
        }
        else
        {
            cout << "\nThe shipment has not been purchased.\n";
        }
    }
    else
    {
        cout << "\nThe total cost of the shipment is higher than the investment amount.\n";
        cout << "The shipment cannot be purchased at this time.\n";
    }
}

int main()
{
    printWelcomeScreen();
    double investmentAmount = getInvestmentAmount();
    int choice;
    Inventory purchasedShipments;

    do
    {
        printMainMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            runSaleManager();
            break;
        case 2:
            runInventoryManager();
            break;
        case 3:
            checkShipments(investmentAmount, purchasedShipments);
            break;
        case 0:
            cout << "Quitting Program...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
