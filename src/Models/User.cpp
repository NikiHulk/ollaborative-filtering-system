#include "User.h"
#include <stdexcept>

/**
 * @brief Конструктор, инициализирующий пользователя по ID.
 * @param id Уникальный идентификатор.
 */
User::User(int id) : id_(id) {}

/**
 * @brief Добавляет новую оценку пользователю.
 *
 * Проверяет соответствие ID пользователя и корректность ID товара.
 * @param rating Оценка для добавления.
 * @throws std::invalid_argument если ID пользователя не совпадает или itemId некорректен.
 */
void User::addRating(const Rating& rating) {
    if (rating.userId != id_) {
        throw std::invalid_argument("Rating user ID does not match this user");
    }

    if (rating.itemId <= 0) {
        throw std::invalid_argument("Invalid item ID in rating");
    }

    ratings_[rating.itemId] = rating;
}

/**
 * @brief Возвращает оценку пользователя для заданного товара.
 * @param itemId ID товара.
 * @return Значение оценки или 0.0, если оценка не найдена.
 */
double User::getRatingForItem(int itemId) const {
    auto it = ratings_.find(itemId);
    return it != ratings_.end() ? it->second.score : 0.0;
}

/**
 * @brief Возвращает все оценки пользователя.
 * @return Хеш-таблица всех оценок.
 */
const std::unordered_map<int, Rating>& User::getRatings() const {
    return ratings_;
}

/**
 * @brief Вычисляет среднее значение всех оценок пользователя.
 * @return Среднее значение или 0.0, если оценок нет.
 */
double User::getAverageRating() const {
    if (ratings_.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& [itemId, r] : ratings_) {
        sum += r.score;
    }
    return sum / ratings_.size();
}
