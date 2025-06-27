#include "Item.h"
#include <stdexcept>

Item::Item(int id) : id_(id) {}

void Item::addRating(const Rating& rating) {
    if (rating.score < 0 || rating.score > 5)
        throw std::invalid_argument("Rating must be 0-5");
    ratings_.push_back(rating);
}

int Item::getId() const {
    return id_;
}

int Item::getRatingCount() const {
    return ratings_.size();
}

double Item::getAverageRating() const {
    if (ratings_.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& rating : ratings_) {
        sum += rating.score;
    }
    return sum / ratings_.size();
}