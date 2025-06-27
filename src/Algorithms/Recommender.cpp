#include "Recommender.h"
#include "Predictor.h"
#include <algorithm>
#include <stdexcept>

namespace recsys {
/**
     * @brief Формирует топ-N рекомендаций для пользователя (user-based подход)
     * 
     * @param userId ID пользователя, для которого формируются рекомендации
     * @param users Вектор всех пользователей системы
     * @param items Вектор всех товаров системы
     * @param N Количество возвращаемых рекомендаций
     * @param k Количество соседей для алгоритма предсказания
     * @param metric Используемая метрика схожести пользователей
     * @return std::vector<std::pair<int, double>> Вектор рекомендаций в формате:
     *         (item_id, predicted_rating), отсортированный по убыванию рейтинга
     * 
     * @throws std::runtime_error Если пользователь не найден в системе
     * 
     * @details Алгоритм:
     * 1. Исключает товары, уже оцененные пользователем
     * 2. Предсказывает рейтинг для каждого неоцененного товара
     * 3. Фильтрует товары с предсказанным рейтингом ≤ 0.0
     * 4. Возвращает топ-N товаров с наивысшим предсказанным рейтингом
     */

    std::vector<std::pair<int, double>> Recommender::recommendTopN(
        int userId,
        const std::vector<User>& users,
        const std::vector<Item>& items,
        int N,
        int k,
        Predictor::Metric metric) {

        const User* user = nullptr;
        for (const auto& u : users) {
            if (u.getId() == userId) {
                user = &u;
                break;
            }
        }
        if (!user) throw std::runtime_error("User not found");
        /**
     * @brief Возвращает топ-N популярных товаров по количеству оценок
     * 
     * @param items Вектор всех товаров системы
     * @param N Количество возвращаемых товаров
     * @return std::vector<std::pair<int, int>> Вектор популярных товаров в формате:
     *         (item_id, rating_count), отсортированный по убыванию количества оценок
     */

        std::vector<std::pair<int, double>> predictions;

        for (const auto& item : items) {
            int itemId = item.getId();
            if (user->getRatingForItem(itemId) > 0.0) continue;

            double predicted = Predictor::predict(userId, itemId, users, k, metric);
            predictions.emplace_back(itemId, predicted);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        predictions.erase(
            std::remove_if(predictions.begin(), predictions.end(),
                           [](auto& p) { return p.second <= 0.0; }),
            predictions.end()
        );

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }
/**
     * @brief Формирует гибридные рекомендации (user-based + item-based)
     * 
     * @param userId ID пользователя, для которого формируются рекомендации
     * @param users Вектор всех пользователей системы
     * @param items Вектор всех товаров системы
     * @param N Количество возвращаемых рекомендаций
     * @param k Количество соседей/товаров для алгоритмов предсказания
     * @param metric Метрика схожести для user-based подхода
     * @param alpha Коэффициент взвешивания (0.0 = только item-based, 1.0 = только user-based)
     * @return std::vector<std::pair<int, double>> Вектор рекомендаций в формате:
     *         (item_id, combined_rating), отсортированный по убыванию рейтинга
     * 
     * @throws std::runtime_error Если пользователь не найден в системе
     * 
     * @details Алгоритм:
     * 1. Для каждого неоцененного товара:
     *    - Вычисляет user-based предсказание
     *    - Вычисляет item-based предсказание
     *    - Комбинирует результаты: combined = alpha*userPred + (1-alpha)*itemPred
     * 2. Возвращает топ-N товаров с наивысшим комбинированным рейтингом
     */

    std::vector<std::pair<int, int>> Recommender::topPopularItems(const std::vector<Item>& items, int N) {
        std::vector<std::pair<int, int>> result;

        for (const auto& item : items) {
            result.emplace_back(item.getId(), item.getRatingCount());
        }

        std::sort(result.begin(), result.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        if (result.size() > N)
            result.resize(N);

        return result;
    }

    std::vector<std::pair<int, double>> Recommender::recommendHybrid(
        int userId,
        const std::vector<User>& users,
        const std::vector<Item>& items,
        int N,
        int k,
        Predictor::Metric metric,
        double alpha) {

        const User* user = nullptr;
        for (const auto& u : users) {
            if (u.getId() == userId) {
                user = &u;
                break;
            }
        }
        if (!user) throw std::runtime_error("User not found");

        std::vector<std::pair<int, double>> predictions;

        for (const auto& item : items) {
            int itemId = item.getId();
            if (user->getRatingForItem(itemId) > 0.0) continue;

            double userPred = Predictor::predict(userId, itemId, users, k, metric);
            double itemPred = Predictor::predictItemBased(userId, itemId, users, items, k);
            double combined = alpha * userPred + (1.0 - alpha) * itemPred;

            predictions.emplace_back(itemId, combined);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        predictions.erase(
            std::remove_if(predictions.begin(), predictions.end(),
                           [](auto& p) { return p.second <= 0.0; }),
            predictions.end()
        );

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }
/**
     * @brief Формирует топ-N рекомендаций (item-based подход)
     * 
     * @param userId ID пользователя, для которого формируются рекомендации
     * @param users Вектор всех пользователей системы
     * @param items Вектор всех товаров системы
     * @param N Количество возвращаемых рекомендаций
     * @return std::vector<std::pair<int, double>> Вектор рекомендаций в формате:
     *         (item_id, predicted_rating), отсортированный по убыванию рейтинга
     * 
     * @throws std::runtime_error Если пользователь не найден в системе
     * 
     * @details Алгоритм:
     * 1. Использует item-based предсказание оценок
     * 2. Возвращает топ-N товаров с наивысшим предсказанным рейтингом
     */

    std::vector<std::pair<int, double>> Recommender::recommendItemBasedTopN(
        int userId,
        const std::vector<User>& users,
        const std::vector<Item>& items,
        int N) {

        const User* user = nullptr;
        for (const auto& u : users) {
            if (u.getId() == userId) {
                user = &u;
                break;
            }
        }
        if (!user) throw std::runtime_error("User not found");

        std::vector<std::pair<int, double>> predictions;

        for (const auto& item : items) {
            int itemId = item.getId();
            if (user->getRatingForItem(itemId) > 0.0) continue;

            double predicted = Predictor::predictItemBased(userId, itemId, users, items);
            predictions.emplace_back(itemId, predicted);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        predictions.erase(
            std::remove_if(predictions.begin(), predictions.end(),
                           [](auto& p) { return p.second <= 0.0; }),
            predictions.end()
        );

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }

}
