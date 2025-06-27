#include "Evaluation.h"
#include <cmath>

namespace recsys {

    double Evaluation::computeMAE(const std::vector<User>& users,
                                   const std::unordered_map<int, std::unordered_map<int, double>>& predicted) {
        double totalError = 0.0;
        int count = 0;

        for (const auto& user : users) {
            int uid = user.getId();
            for (const auto& [itemId, actual] : user.getRatings()) {
                auto itUser = predicted.find(uid);
                if (itUser != predicted.end()) {
                    auto itItem = itUser->second.find(itemId);
                    if (itItem != itUser->second.end()) {
                        totalError += std::abs(actual.score - itItem->second);
                        count++;
                    }
                }
            }
        }
        return (count > 0) ? totalError / count : 0.0;
    }

    double Evaluation::computeRMSE(const std::vector<User>& users,
                                   const std::unordered_map<int, std::unordered_map<int, double>>& predicted) {
        double totalSquaredError = 0.0;
        int count = 0;

        for (const auto& user : users) {
            int uid = user.getId();
            for (const auto& [itemId, actual] : user.getRatings()) {
                auto itUser = predicted.find(uid);
                if (itUser != predicted.end()) {
                    auto itItem = itUser->second.find(itemId);
                    if (itItem != itUser->second.end()) {
                        double diff = actual.score - itItem->second;
                        totalSquaredError += diff * diff;
                        count++;
                    }
                }
            }
        }
        return (count > 0) ? std::sqrt(totalSquaredError / count) : 0.0;
    }

}
