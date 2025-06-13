#include "Predictor.h"
#include <algorithm>
#include <stdexcept>

namespace recsys {

    double Predictor::predict(int userId, int itemId,
                              const std::vector<User>& users,
                              int k) {
        //Находим указатель целевого юзера
        const User* target = nullptr;
        for (auto const& u : users) {
            if (u.getId() == userId) {
                target = &u;
                break;
            }
        }
        if (!target) {
            throw std::runtime_error("User not found");
        }

        // Собираем сходства с помощью вектора
        std::vector<std::pair<double, const User*>> sims;
        for (auto const& u : users) {
            if (u.getId() == userId) continue;
            double s = Similarity::cosine(*target, u);
            sims.emplace_back(s, &u);
        }

        //Сорт по убыванию
        std::sort(sims.begin(), sims.end(),
                  [](auto& a, auto& b){ return a.first > b.first; });

        //Берем до k нужных соседей
        double num = 0.0, den = 0.0;
        int count = 0;
        for (auto const& [sim, userPtr] : sims) {
            if (sim <= 0) break;
            double r = userPtr->getRatingForItem(itemId);
            //если сосед не поставил оценку, скип
            if (r < 0) continue;
            num += sim * r;
            den += sim;
            if (++count >= k) break;
        }

        if (den == 0) {
            return 0.0;
        }
        return num / den;
    }

}
