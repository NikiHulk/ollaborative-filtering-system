#include "Recommender.h"
#include "Predictor.h"
#include <algorithm>
#include <stdexcept>

namespace recsys {

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

        std::vector<std::pair<int, double>> predictions;

        for (const auto& item : items) {
            int itemId = item.getId();
            if (user->getRatingForItem(itemId) > 0.0) continue;
            double predicted = Predictor::predict(userId, itemId, users, k, metric);
            if (predicted > 0.0)
                predictions.emplace_back(itemId, predicted);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }

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
            if (combined > 0.0)
                predictions.emplace_back(itemId, combined);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }

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
            if (predicted > 0.0)
                predictions.emplace_back(itemId, predicted);
        }

        std::sort(predictions.begin(), predictions.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        if (predictions.size() > N)
            predictions.resize(N);

        return predictions;
    }

}
