#include "User.h"

User::User(int id) : id_(id) {}

void User::addRating(const Rating& rating) {
    ratings_[rating.itemId] = rating;
}

double User::getRatingForItem(int itemId) const {
    auto it = ratings_.find(itemId);
    return it != ratings_.end() ? it->second.score : 0.0;
}

const std::unordered_map<int, Rating>& User::getRatings() const {
    return ratings_;
}