#pragma once
#include <vector>

class Item {
public:
    Item(int id);
    
    void addRating(const Rating& rating);
    double getAverageRating() const;
    
private:
    int id_;
    std::vector<Rating> ratings_;
};