#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product
{
public:
    std::string name;
    double price, cost;
    int quantity;
    std::string category;
    std::string expDate;

    Product() : Product("", 0, 0, 0, "", "N/A") {}

    // I just add cost to keep track of our input cost and maybe to calculate sales and profit later
    Product(std::string name,
            double price,
            double cost,
            int quantity,
            std::string category,
            std::string expDate)
    {
        this->name = name;
        this->price = price;
        this->cost = cost;
        this->quantity = quantity;
        this->category = category;
        this->expDate = expDate;
    }

    bool isExpired();

    // took me hour to align these items to look nice
    void print() const;
};

#endif
