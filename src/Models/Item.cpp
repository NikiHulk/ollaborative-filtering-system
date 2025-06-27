/**
* @file Item.h
 * @brief Класс модели Item (товар), содержащий список оценок от пользователей.
 */

#pragma once

#include <vector>
#include "Rating.h"

/**
 * @class Item
 * @brief Представляет товар, который может быть оценён пользователями.
 *
 * Хранит идентификатор и список оценок от разных пользователей.
 */
class Item {
public:
    /**
     * @brief Конструктор по идентификатору.
     * @param id Уникальный идентификатор товара.
     */
    Item(int id);

    /**
     * @brief Добавляет новую оценку к товару.
     * @param rating Объект оценки типа Rating.
     * @throw std::invalid_argument Если значение оценки вне диапазона [0; 5].
     */
    void addRating(const Rating& rating);

    /**
     * @brief Получает идентификатор товара.
     * @return Целочисленный ID.
     */
    int getId() const;

    /**
     * @brief Получает количество оценок для этого товара.
     * @return Количество оценок (размер вектора).
     */
    int getRatingCount() const;

    /**
     * @brief Возвращает среднюю оценку по всем полученным рейтингам.
     * @return Среднее арифметическое значений оценок. 0.0 если оценок нет.
     */
    double getAverageRating() const;

private:
    int id_;  ///< Идентификатор товара.
    std::vector<Rating> ratings_;  ///< Список оценок, оставленных пользователями.
};
