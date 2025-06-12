#pragma once
#include <vector>
#include <unordered_map>
#include "Rating.h"

class User {
public:
    User(int id);
    
    void addRating(const Rating& rating);
    double getRatingForItem(int itemId) const;
    const std::unordered_map<int, Rating>& getRatings() const;
    
private:
    int id_;
    std::unordered_map<int, Rating> ratings_; //itemID --> ratingID
};