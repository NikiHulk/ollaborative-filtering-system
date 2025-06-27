#pragma once

#include "../Models/User.h"
#include "../Models/Item.h"
#include "Predictor.h"
#include <vector>
#include <utility>

namespace recsys {

    class Recommender {
    public:
        /// Вернуть N наилучших рекомендаций пользователю userId.
        /// Возвращает вектор (itemId, predictedRating) отсортированный по убыванию рейтинга.
        static std::vector<std::pair<int, double>> recommendTopN(
            int userId,
            const std::vector<User>& users,
            const std::vector<Item>& items,
            int N = 5,
            int k = 5,
            Predictor::Metric metric = Predictor::Metric::Cosine);

        /// Вернуть top-N самых популярных товаров по количеству оценок
        static std::vector<std::pair<int, int>> topPopularItems(
            const std::vector<Item>& items, int N = 5);

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

        static std::vector<std::pair<int, double>> recommendItemBasedTopN(
            int userId,
            const std::vector<User>& users,
            const std::vector<Item>& items,
            int N
        );

    };

}
