#include "Item.h"
#include <stdexcept>

Item::Item(int id) : id_(id) {}  // Явная инициализация

void Item::addRating(const Rating& rating) {
    if (rating.score < 0 || rating.score > 5)
        throw std::invalid_argument("Rating must be 0-5");
    ratings_.push_back(rating);
}

int Item::getId() const {
    return id_; // предположим, что у вас есть private член id_
}

double Item::getAverageRating() const {
    if (ratings_.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& rating : ratings_) {
        sum += rating.score;
    }
    return sum / ratings_.size();
}