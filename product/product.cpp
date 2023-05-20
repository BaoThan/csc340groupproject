#include "product.hpp"

#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;


bool Product::isExpired()
{
    struct tm _;
    if (!strptime(expDate.c_str(), "%Y-%m-%d", &_)) return false;

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

void Product::print() const
{
    cout << left << setw(30) << name;
    cout << right << setw(5) << fixed << setprecision(2) << "$ " << left << setw(15) << price;
    cout << left << setw(10) << quantity;
    cout << right << setw(10) << category << endl;
    cout << right << setw(15) << expDate << endl;
}
