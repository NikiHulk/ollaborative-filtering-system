#include "Item.h"

Item::Item(int id) : id_(id) {}

void Item::addRating(const Rating& rating) {
    ratings_.push_back(rating);
}

double Item::getAverageRating() const {
    if (ratings_.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& rating : ratings_) {
        sum += rating.score;
    }
    return sum / ratings_.size();
}