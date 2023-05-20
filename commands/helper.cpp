#include "helper.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

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

void readInventoryFromFile(Inventory &inventory, const string &filename)
{
    // I think we dont need this?
    //  Get the current directory path
    fs::path currentPath = fs::current_path();

    // Go back to the parent directory (main folder)
    fs::path parentPath = currentPath.parent_path();

    // Combine the parent directory path with the filename
    fs::path filePath = parentPath / filename;

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
                    outFile << product.name() << ","
                            << product.price() << ","
                            << product.cost() << ","
                            << product.quantity() << ","
                            << product.category() << ","
                            << product.expDate() << endl;
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
                Node *expiredNode = inventory.searchProduct(product.name());
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
            categories.insert(current->product.category());
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
            if (current->product.category() == selectedCategory)
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
        existingProduct->product.setPrice(price);
        existingProduct->product.setCost(cost);
        existingProduct->product.setQuantity(quantity);
        existingProduct->product.setCategory(category);
        existingProduct->product.setExpDate(expDate);

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
