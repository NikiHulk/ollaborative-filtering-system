#include "User.h"
#include <stdexcept>

User::User(int id) : id_(id) {}

void User::addRating(const Rating& rating) {
    //Проверка ID юзера
    if (rating.userId != id_) {
        throw std::invalid_argument("Rating user ID does not match this user");
    }

    //Проверка ItemID
    if (rating.itemId <= 0) {
        throw std::invalid_argument("Invalid item ID in rating");
    }

    ratings_[rating.itemId] = rating;
}
double User::getRatingForItem(int itemId) const {
    auto it = ratings_.find(itemId);
    return it != ratings_.end() ? it->second.score : 0.0;
}

const std::unordered_map<int, Rating>& User::getRatings() const {
    return ratings_;
}

double User::getAverageRating() const {
    if (ratings_.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& [itemId, r] : ratings_) {
        sum += r.score;
    }
    return sum / ratings_.size();
}
