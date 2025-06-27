#pragma once
#include <vector>
#include "Rating.h"

class Item {
public:
    explicit Item(int id);
    void addRating(const Rating& rating);
    double getAverageRating() const;
    int getId() const;
    int getRatingCount() const;

    
private:
    int id_;
    std::vector<Rating> ratings_;
};