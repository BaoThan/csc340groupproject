#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product
{
public:
    Product() : Product("", 0, 0, 0, "", "N/A") {}

    // I just add cost to keep track of our input cost and maybe to calculate sales and profit later
    Product(std::string name,
            double price,
            double cost,
            int quantity,
            std::string category,
            std::string expDate) : _name(name),
                                   _price(price),
                                   _cost(cost),
                                   _quantity(quantity),
                                   _category(category),
                                   _expDate(expDate) {}

    std::string name() const { return _name; }

    void setName(std::string name) { _name = name; }

    double price() const { return _price; }

    void setPrice(double price) { _price = price; }

    double cost() const { return _cost; }

    void setCost(double cost) { _cost = cost; }

    int quantity() const { return _quantity; }

    void setQuantity(int quantity) { _quantity = quantity; }

    std::string category() const { return _category; }

    void setCategory(std::string category) { _category = category; }

    std::string expDate() const { return _expDate; }

    void setExpDate(std::string expDate) { _expDate = expDate; }

    bool isExpired();

    void print() const;

private:
    std::string _name;
    double _price;
    double _cost;
    int _quantity;
    std::string _category;
    std::string _expDate;
};

#endif
