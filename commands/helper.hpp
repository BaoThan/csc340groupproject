#ifndef COMMANDS_HELPER_HPP
#define COMMANDS_HELPER_HPP

#include "../inventory/inventory.hpp"

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#  endif
#endif

void readInventoryFromFile(Inventory &inventory, const string &filename);

void printMenu();

void printSortMenu();

void filterExpiredProducts(Inventory &inventory);

void displayProductsByCategoryMenu(Inventory &inventory);

void updateQuantity(Inventory &inventory);

void addProduct(Inventory &inventory);

void deleteProduct(Inventory &inventory);

void searchProduct(Inventory &inventory);

void updatePrice(Inventory &inventory);

#endif
