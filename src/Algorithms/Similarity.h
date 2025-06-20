#pragma once

#include "../Models/User.h"
#include "../Models/Rating.h"
#include <cmath>
#include <vector>
#include <utility>

namespace recsys {

    ///Статические методы для расчёта метрик сходства пользователей.
    class Similarity {
    public:
        ///Косинусная близость
        static double cosine(const User& u1, const User& u2);

        ///Корреляция Пирсона
        static double pearson(const User& u1, const User& u2);

        ///Жаккард(для бинарных данных)
        static double jaccard(const User& u1, const User& u2);
    };

}
