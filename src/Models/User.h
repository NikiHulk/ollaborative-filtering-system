#pragma once

#include "Rating.h"
#include <unordered_map>

/**
 * @class User
 * @brief Класс представляет пользователя и его оценки.
 */
class User {
public:
    /**
     * @brief Конструктор по ID.
     * @param id Уникальный идентификатор пользователя.
     */
    explicit User(int id);

    /**
     * @brief Добавляет оценку пользователю.
     * @param rating Объект Rating с ID пользователя, ID товара и оценкой.
     * @throws std::invalid_argument Если ID пользователя или товара некорректен.
     */
    void addRating(const Rating& rating);

    /**
     * @brief Получает оценку пользователя для конкретного товара.
     * @param itemId ID товара.
     * @return Оценка, если найдена, иначе 0.0.
     */
    double getRatingForItem(int itemId) const;

    /**
     * @brief Получает все оценки пользователя.
     * @return Словарь itemId → Rating.
     */
    const std::unordered_map<int, Rating>& getRatings() const;

    /**
     * @brief Вычисляет среднюю оценку пользователя.
     * @return Среднее значение всех оценок. Если нет оценок, вернёт 0.0.
     */
    double getAverageRating() const;

    /**
     * @brief Получает ID пользователя.
     * @return Целочисленный ID.
     */
    int getId() const { return id_; }

private:
    int id_;  ///< Уникальный ID пользователя
    std::unordered_map<int, Rating> ratings_; ///< Оценки: itemId → Rating
};
