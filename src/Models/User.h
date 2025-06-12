#pragma once
#include <unordered_map>
#include "Rating.h" // Явное включение

class User {
public:
    explicit User(int id);
    void addRating(const Rating& rating);
    double getRatingForItem(int itemId) const;
    const std::unordered_map<int, Rating>& getRatings() const;

private:
    int id_;
    std::unordered_map<int, Rating> ratings_; // itemId -> Rating
};