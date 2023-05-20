#include "commands/commands.hpp"
#include "inventory/inventory.hpp"
#include "product/product.hpp"
#include "utils/utils.hpp"

#include <iostream>


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
            std::cout << "Quitting Program...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
