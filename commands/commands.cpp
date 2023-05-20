#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "helper.hpp"


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
    if (!fs::exists(directory))
    {
        cout << "Directory does not exist: " << directory << endl;
        return;
    }

    // Create a vector to store file paths
    vector<string> fileList;

    // Iterate over files in the directory
    for (const auto &entry : fs::directory_iterator(directory))
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
