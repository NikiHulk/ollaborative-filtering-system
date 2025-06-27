#pragma once

#include "../Models/User.h"
#include "Similarity.h"
#include "../Models/Item.h"
#include <vector>

namespace recsys {
/**
     * @class Predictor
     * @brief Класс для предсказания пользовательских оценок
     * 
     * Реализует алгоритмы предсказания на основе:
     * - User-based коллаборативной фильтрации
     * - Item-based коллаборативной фильтрации
     * 
     * Использует кэширование результатов для ускорения расчетов
     */

    /// Предсказание оценок на основе k-NN.
    class Predictor {
    public:
/**
         * @enum Metric
         * @brief Метрики схожести пользователей
         */
        /// Доступные метрики сходства
        enum class Metric {
            Cosine,
            Pearson,
            Jaccard
        };
/**
         * @brief Предсказание оценки (user-based подход)
         * 
         * @param userId ID целевого пользователя
         * @param itemId ID целевого товара
         * @param users Вектор всех пользователей системы
         * @param k Количество ближайших соседей (по умолчанию 5)
         * @param metric Метрика схожести (по умолчанию Cosine)
         * @return double Предсказанная оценка в диапазоне [0, 1]
         * 
         * @details Алгоритм:
         * 1. Находит k пользователей с наибольшей схожестью
         * 2. Усредняет их оценки целевого товара
         * 3. Возвращает 0.0 если:
         *    - Пользователь не найден
         *    - Нет подходящих соседей
         *    - Все веса ≤ 0
         */

        /// Предсказать оценку для userId на itemId,
        /// беря в расчёт k самых похожих пользователей и метрику metric.
        /// Вернёт 0.0, если не удалось предсказать (нет соседей или все веса ≤ 0).
        static double predict(int userId,
                              int itemId,
                              const std::vector<User>& users,
                              int k = 5,
                              Metric metric = Metric::Cosine);
/**
         * @brief Предсказание оценки (item-based подход)
         * 
         * @param userId ID целевого пользователя
         * @param itemId ID целевого товара
         * @param users Вектор всех пользователей системы
         * @param items Вектор всех товаров системы
         * @param k Количество ближайших товаров (по умолчанию 5)
         * @return double Предсказанная оценка в диапазоне [0, 1]
         * 
         * @details Алгоритм:
         * 1. Для товаров, оцененных пользователем:
         *    - Вычисляет схожесть с целевым товаром
         * 2. Выбирает k наиболее схожих товаров
         * 3. Усредняет их оценки
         */

        static double predictItemBased(int userId, int itemId,
                               const std::vector<User>& users,
                               const std::vector<Item>& items,
                               int k = 5);
    };

} // namespace recsys
