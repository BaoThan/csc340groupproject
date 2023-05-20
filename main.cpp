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

#include "commands/commands.hpp"
#include "inventory/inventory.hpp"
#include "product/product.hpp"
#include "utils/utils.hpp"

using namespace std;


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
