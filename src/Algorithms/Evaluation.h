#pragma once
#include <vector>
#include <unordered_map>
#include <Models/User.h>

namespace recsys {
    /**
     * @class Evaluation
     * @brief Класс для оценки качества рекомендательных систем
     * 
     * Предоставляет статические методы для расчета метрик точности предсказаний
     */

    class Evaluation {
    public:
        /**
         * @brief Вычисляет среднюю абсолютную ошибку (MAE)
         * 
         * @param users Вектор пользователей с фактическими оценками
         * @param predicted Словарь предсказаний в формате:
         *        { user_id: { item_id: predicted_rating } }
         * @return double Средняя абсолютная ошибка. Возвращает 0.0 если:
         *        - Нет совпадающих пар (user_id, item_id)
         *        - Списки пользователей/предсказаний пусты
         */
        static double computeMAE(const std::vector<User>& users,
                                 const std::unordered_map<int, std::unordered_map<int, double>>& predicted);
        /**
         * @brief Вычисляет среднеквадратичную ошибку (RMSE)
         * 
         * @param users Вектор пользователей с фактическими оценками
         * @param predicted Словарь предсказаний в формате:
         *        { user_id: { item_id: predicted_rating } }
         * @return double Корень из средней квадратичной ошибки. Возвращает 0.0 если:
         *        - Нет совпадающих пар (user_id, item_id)
         *        - Списки пользователей/предсказаний пусты
         */

        static double computeRMSE(const std::vector<User>& users,
                                  const std::unordered_map<int, std::unordered_map<int, double>>& predicted);
    };

}
