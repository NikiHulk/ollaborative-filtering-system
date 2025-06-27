#pragma once

#include "../Models/User.h"
#include "../Models/Item.h"
#include "Predictor.h"
#include <vector>
#include <utility>

namespace recsys {
/**
     * @class Recommender
     * @brief Класс для генерации рекомендаций пользователям
     * 
     * Предоставляет различные стратегии формирования рекомендаций:
     * - User-based коллаборативная фильтрация
     * - Item-based коллаборативная фильтрация
     * - Гибридные подходы
     * - Популярные товары
     */

    class Recommender {
    public:
/**
         * @brief Генерирует топ-N рекомендаций (user-based подход)
         * 
         * @param userId ID пользователя, для которого формируются рекомендации
         * @param users Вектор всех пользователей системы
         * @param items Вектор всех товаров системы
         * @param N Количество возвращаемых рекомендаций (по умолчанию 5)
         * @param k Количество соседей для алгоритма предсказания (по умолчанию 5)
         * @param metric Метрика схожести пользователей (по умолчанию Cosine)
         * @return std::vector<std::pair<int, double>> Вектор пар (item_id, predicted_rating)
         *         отсортированный по убыванию предсказанного рейтинга
         */
        /// Вернуть N наилучших рекомендаций пользователю userId.
        /// Возвращает вектор (itemId, predictedRating) отсортированный по убыванию рейтинга.
        static std::vector<std::pair<int, double>> recommendTopN(
            int userId,
            const std::vector<User>& users,
            const std::vector<Item>& items,
            int N = 5,
            int k = 5,
            Predictor::Metric metric = Predictor::Metric::Cosine);
/**
         * @brief Возвращает топ-N самых популярных товаров
         * 
         * @param items Вектор всех товаров системы
         * @param N Количество возвращаемых товаров (по умолчанию 5)
         * @return std::vector<std::pair<int, int>> Вектор пар (item_id, rating_count)
         *         отсортированный по убыванию количества оценок
         */

        /// Вернуть top-N самых популярных товаров по количеству оценок
        static std::vector<std::pair<int, int>> topPopularItems(
            const std::vector<Item>& items, int N = 5);
/**
         * @brief Генерирует гибридные рекомендации (user-based + item-based)
         * 
         * @param userId ID пользователя, для которого формируются рекомендации
         * @param users Вектор всех пользователей системы
         * @param items Вектор всех товаров системы
         * @param N Количество возвращаемых рекомендаций
         * @param k Количество соседей/товаров для алгоритмов предсказания
         * @param metric Метрика схожести для user-based подхода
         * @param alpha Коэффициент взвешивания:
         *              - 0.0 = только item-based
         *              - 1.0 = только user-based
         * @return std::vector<std::pair<int, double>> Вектор пар (item_id, combined_rating)
         *         отсортированный по убыванию комбинированного рейтинга
         */

        /// Гибридная рекомендация: объединяет user-based и item-based подходы.
        /// alpha — вес user-based предсказания (0.0 = только item-based, 1.0 = только user-based)
        static std::vector<std::pair<int, double>> recommendHybrid(
            int userId,
            const std::vector<User>& users,
            const std::vector<Item>& items,
            int N,
            int k,
            Predictor::Metric metric,
            double alpha
            );
/**
         * @brief Генерирует топ-N рекомендаций (item-based подход)
         * 
         * @param userId ID пользователя, для которого формируются рекомендации
         * @param users Вектор всех пользователей системы
         * @param items Вектор всех товаров системы
         * @param N Количество возвращаемых рекомендаций
         * @return std::vector<std::pair<int, double>> Вектор пар (item_id, predicted_rating)
         *         отсортированный по убыванию предсказанного рейтинга
         */

        static std::vector<std::pair<int, double>> recommendItemBasedTopN(
            int userId,
            const std::vector<User>& users,
            const std::vector<Item>& items,
            int N
        );

    };

}
