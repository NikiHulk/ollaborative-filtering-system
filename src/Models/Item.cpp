#include "Item.h"
#include <stdexcept>

namespace recsys {

    Item::Item(int id) : id_(id) {}

    void Item::addRating(const Rating& rating) {
        if (rating.itemId != id_) {
            throw std::invalid_argument("Rating item ID does not match this item");
        }

        if (rating.score < 0.0 || rating.score > 5.0) {
            throw std::invalid_argument("Rating score must be in [0, 5]");
        }

        ratings_.push_back(rating);
    }

    double Item::getAverageRating() const {
        if (ratings_.empty()) return 0.0;

        double sum = 0.0;
        for (const auto& r : ratings_) {
            sum += r.score;
        }
        return sum / ratings_.size();
    }

    int Item::getId() const {
        return id_;
    }

    int Item::getRatingCount() const {
        return static_cast<int>(ratings_.size());
    }

} // namespace recsys
