#pragma once
#include <unordered_map>
#include "Rating.h"

class User {
public:
    explicit User(int id);
    int getId() const { return id_; }
    void addRating(const Rating& rating);
    double getRatingForItem(int itemId) const;
    const std::unordered_map<int, Rating>& getRatings() const;
    double getAverageRating() const;
private:
    int id_;
    std::unordered_map<int, Rating> ratings_;
};
