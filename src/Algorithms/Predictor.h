#pragma once

#include "../Models/User.h"
#include "Similarity.h"
#include <vector>

namespace recsys {

    /// Предсказание оценок на основе k-NN.
    class Predictor {
    public:
        /// Доступные метрики сходства
        enum class Metric {
            Cosine,
            Pearson,
            Jaccard
        };

        /// Предсказать оценку для userId на itemId,
        /// беря в расчёт k самых похожих пользователей и метрику metric.
        /// Вернёт 0.0, если не удалось предсказать (нет соседей или все веса ≤ 0).
        static double predict(int userId,
                              int itemId,
                              const std::vector<User>& users,
                              int k = 5,
                              Metric metric = Metric::Cosine);
    };

} // namespace recsys
