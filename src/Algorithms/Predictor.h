#pragma once

#include "../Models/User.h"
#include "../Models/Item.h"
#include "Similarity.h"
#include <vector>

namespace recsys {

    ///Класс для предсказания оценок на основе k ближайших юзеров.
    class Predictor {
    public:
        ///Предсказать оценку для userId на itemId,
        ///беря в расчёт k самых похожих пользователей.
        ///users — список всех загруженных пользователей,
        ///items — список всех загруженных товаров.
        static double predict(int userId, int itemId,
                              const std::vector<User>& users,
                              int k = 5);
    };

}
