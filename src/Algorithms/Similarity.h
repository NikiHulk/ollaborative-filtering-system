#pragma once

#include <vector>
#include <unordered_map>
#include "User.h"
#include "Item.h"

namespace recsys {

    /// Интерфейс для вычисления метрики сходства между двумя пользователями
    class Similarity {
    public:
        /// Косинусная близость
        static double cosine(const User& u1, const User& u2);

        /// Корреляция Пирсона
        static double pearson(const User& u1, const User& u2);

        /// Jaccard (для бинарных данных, опционально)
        static double jaccard(const User& u1, const User& u2);
    };

} // namespace recsys
