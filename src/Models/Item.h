/**
* @file Item.h
 * @brief Заголовочный файл для класса Item, представляющего товар с оценками.
 */

#pragma once

#include <vector>
#include "Rating.h"

namespace recsys {  // ✅ Добавить пространство имён

    /**
     * @class Item
     * @brief Класс, описывающий товар (элемент), который может быть оценен пользователями.
     *
     * Каждый товар имеет уникальный идентификатор и список оценок (Rating).
     * Предоставляет методы для добавления оценок и получения статистики.
     */
    class Item {
    public:
        explicit Item(int id);
        void addRating(const Rating& rating);
        double getAverageRating() const;
        int getId() const;
        int getRatingCount() const;

    private:
        int id_;  ///< Уникальный идентификатор товара.
        std::vector<Rating> ratings_; ///< Список всех оценок, оставленных пользователями.
    };

} // namespace recsys
