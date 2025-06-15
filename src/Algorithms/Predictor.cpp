#include "Predictor.h"
#include <algorithm>
#include <stdexcept>

namespace recsys {

    double Predictor::predict(int userId,
                              int itemId,
                              const std::vector<User>& users,
                              int k,
                              Metric metric) {
        auto getSim = [&](const User& a, const User& b) {
            switch (metric) {
                case Metric::Cosine:  return Similarity::cosine(a, b);
                case Metric::Pearson: return Similarity::pearson(a, b);
                case Metric::Jaccard: return Similarity::jaccard(a, b);
            }
            return 0.0;
        };

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
        std::vector<std::pair<double, const User*>> sims;
        sims.reserve(users.size() - 1);
        for (auto const& u : users) {
            if (u.getId() == userId) continue;
            double s = getSim(*target, u);
            sims.emplace_back(s, &u);
        }
        std::sort(sims.begin(), sims.end(),
                  [](auto &a, auto &b) { return a.first > b.first; });
        double num = 0.0, den = 0.0;
        int taken = 0;
        for (auto const& [sim, u_ptr] : sims) {
            if (sim <= 0.0) break;
            double r = u_ptr->getRatingForItem(itemId);
            if (r <= 0.0) continue;
            num += sim * r;
            den += sim;
            if (++taken >= k) break;
        }
        return den > 0.0 ? num / den : 0.0;
    }

}
