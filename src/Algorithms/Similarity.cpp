#include "Algorithms/Similarity.h"
#include "../Models/User.h"


namespace recsys {
/**
     * @brief Вычисляет косинусную схожесть между двумя пользователями
     * 
     * @param u1 Первый пользователь
     * @param u2 Второй пользователь
     * @return double Значение схожести в диапазоне [0, 1]
     * 
     * @details Формула:
     * \f[
     * \text{similarity} = \frac{u_1 \cdot u_2}{\|u_1\| \times \|u_2\|}
     * \f]
     * где:
     * - \f$u_1 \cdot u_2\f$ - скалярное произведение оценок общих товаров
     * - \f$\|u_1\|\f$ и \f$\|u_2\|\f$ - нормы векторов оценок
     * Возвращает 0.0 если один из векторов нулевой
     */

    double Similarity::cosine(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();

        double dot = 0.0, norm1 = 0.0, norm2 = 0.0;
        for (auto& [item, rating] : r1) {
            norm1 += rating.score * rating.score;
            auto it = r2.find(item);
            if (it != r2.end()) {
                dot += rating.score * it->second.score;
            }
        }
        for (auto& [item, rating] : r2) {
            norm2 += rating.score * rating.score;
        }
        if (norm1 == 0.0 || norm2 == 0.0) return 0.0;
        return dot / (std::sqrt(norm1) * std::sqrt(norm2));
    }
/**
     * @brief Вычисляет корреляцию Пирсона между двумя пользователями
     * 
     * @param u1 Первый пользователь
     * @param u2 Второй пользователь
     * @return double Значение корреляции в диапазоне [-1, 1]
     * 
     * @details Особенности:
     * - Использует только общие товары
     * - Возвращает 0.0 если нет общих товаров
     * - Возвращает 1.0 если только один общий товар
     * - Формула:
     * \f[
     * r = \frac{\sum (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\sum (x_i - \bar{x})^2 \sum (y_i - \bar{y})^2}}
     * \f]
     */

    double Similarity::pearson(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();

        std::vector<std::pair<double,double>> common;
        for (auto& [item, rating1] : r1) {
            auto it = r2.find(item);
            if (it != r2.end()) {
                common.emplace_back(rating1.score, it->second.score);
            }
        }
        int n = common.size();
        if (n == 0) return 0.0;
        if (n == 1) return 1.0;

        double sum1=0, sum2=0, sum1Sq=0, sum2Sq=0, pSum=0;
        for (auto& [x,y] : common) {
            sum1   += x;
            sum2   += y;
            sum1Sq += x*x;
            sum2Sq += y*y;
            pSum   += x*y;
        }
        double num = pSum - (sum1 * sum2 / n);
        double den = std::sqrt((sum1Sq - sum1*sum1/n) * (sum2Sq - sum2*sum2/n));
        return (den == 0.0) ? 0.0 : num/den;
    }
/**
     * @brief Вычисляет схожесть Жаккара между двумя пользователями
     * 
     * @param u1 Первый пользователь
     * @param u2 Второй пользователь
     * @return double Коэффициент Жаккара в диапазоне [0, 1]
     * 
     * @details Формула:
     * \f[
     * J = \frac{|A \cap B|}{|A \cup B|}
     * \f]
     * где:
     * - \f$A\f$ - множество товаров первого пользователя
     * - \f$B\f$ - множество товаров второго пользователя
     * Возвращает 0.0 если нет общих товаров
     */

    double Similarity::jaccard(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();
        int inter = 0;
        for (auto& [item, _] : r1) {
            if (r2.count(item)) inter++;
        }
        int uni = r1.size() + r2.size() - inter;
        return uni == 0 ? 0.0 : static_cast<double>(inter)/uni;
    }
/**
     * @brief Вычисляет скорректированную косинусную схожесть между двумя товарами
     * 
     * @param users Вектор всех пользователей системы
     * @param itemId1 ID первого товара
     * @param itemId2 ID второго товара
     * @return double Значение схожести в диапазоне [-1, 1]
     * 
     * @details Особенности:
     * - Учитывает средние оценки пользователей
     * - Формула:
     * \f[
     * \text{sim}(i,j) = \frac{
     *   \sum\limits_{u \in U} (r_{u,i} - \bar{r_u}) (r_{u,j} - \bar{r_u})
     * }{
     *   \sqrt{\sum\limits_{u \in U} (r_{u,i} - \bar{r_u})^2} 
     *   \sqrt{\sum\limits_{u \in U} (r_{u,j} - \bar{r_u})^2}
     * }
     * \f]
     * Возвращает 0.0 если нет пользователей, оценивших оба товара
     */

    double Similarity::adjustedCosine(const std::vector<User>& users, int itemId1, int itemId2) {
        std::vector<std::pair<double, double>> common;

        for (const auto& user : users) {
            double r1 = user.getRatingForItem(itemId1);
            double r2 = user.getRatingForItem(itemId2);

            if (r1 > 0.0 && r2 > 0.0) {
                double avg = user.getAverageRating();  // уже реализовано
                common.emplace_back(r1 - avg, r2 - avg);
            }
        }

        if (common.empty()) return 0.0;

        double num = 0.0, den1 = 0.0, den2 = 0.0;
        for (auto& [a, b] : common) {
            num += a * b;
            den1 += a * a;
            den2 += b * b;
        }

        double den = std::sqrt(den1) * std::sqrt(den2);
        return (den == 0.0) ? 0.0 : num / den;
    }
/**
     * @brief Вычисляет манхэттенское расстояние между пользователями
     * 
     * @param u1 Первый пользователь
     * @param u2 Второй пользователь
     * @return double Нормализованная схожесть в диапазоне [0, 1]
     * 
     * @details Преобразование расстояния в схожесть:
     * \f[
     * \text{similarity} = \frac{1}{1 + \text{distance}}
     * \f]
     * Возвращает 0.0 если нет общих товаров
     */

    double Similarity::manhattan(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();
        double sum = 0.0;
        int count = 0;
        for (auto& [item, rating] : r1) {
            auto it = r2.find(item);
            if (it != r2.end()) {
                sum += std::abs(rating.score - it->second.score);
                count++;
            }
        }
        return count == 0 ? 0.0 : 1.0 / (1.0 + sum);
    }
/**
     * @brief Вычисляет весовой коэффициент с временным затуханием
     * 
     * @param timestamp Время оценки (в секундах с эпохи UNIX)
     * @param now Текущее время (в секундах с эпохи UNIX)
     * @param halfLife Период полураспада (в секундах)
     * @return double Весовой коэффициент в диапазоне (0, 1]
     * 
     * @details Формула экспоненциального затухания:
     * \f[
     * w = e^{-\frac{\text{age}}{\text{halfLife}}}
     * \f]
     * где:
     * - \f$\text{age} = \text{now} - \text{timestamp}\f$
     */

    double Similarity::decayWeight(long timestamp, long now, double halfLife) {
        double age = std::max(0.0, static_cast<double>(now - timestamp));
        return std::exp(-age / halfLife);
    }


}
