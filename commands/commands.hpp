#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "../inventory/inventory.hpp"

int getMenuChoice();

void runSaleManager();

void runInventoryManager();

void checkShipments(double &investmentAmount, Inventory &purchasedShipments);

#endif
