#include "utils.hpp"

#include <chrono>
#include <iostream>

using namespace std;

int getMenuChoice()
{
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cout << "\n";
    return choice;
}

void printMainMenu()
{
    cout << "Please select an option:\n";
    cout << "1. Sale Manager\n";
    cout << "2. Inventory Manager\n";
    cout << "3. Check Shipments\n";
    cout << "0. Quit\n";
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

double getInvestmentAmount()
{
    double amount;
    cout << "Enter your investment amount in dollars: $";
    cin >> amount;
    cout << "\n";
    return amount;
}
