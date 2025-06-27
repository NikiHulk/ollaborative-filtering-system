/**
* @file EvaluationTest.cpp
 * @brief Тесты для проверки корректности вычисления метрик MAE и RMSE.
 */

#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Evaluation.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>

using namespace Catch;
using namespace recsys;
using Catch::Approx;

/**
 * @test Проверка вычисления метрик MAE и RMSE.
 *
 * Создаётся один пользователь с одной реальной оценкой,
 * и тестируются:
 * - средняя абсолютная ошибка (MAE)
 * - среднеквадратичная ошибка (RMSE)
 */
TEST_CASE("Evaluation metrics compute correctly") {
    std::vector<User> users;

    // Создаём пользователя с одной реальной оценкой: item 101 оценён на 5.0
    // Предсказание: 4.5
    User u1(1);
    u1.addRating(Rating(1, 101, 5.0, 123456789));
    users.push_back(u1);

    // Предсказанные оценки: user 1 → item 101 → 4.5
    std::unordered_map<int, std::unordered_map<int, double>> predictions = {
        {1, {{101, 4.5}}}
    };

    /**
     * @brief Тестирует вычисление MAE.
     *
     * MAE = |5.0 - 4.5| = 0.5
     */
    SECTION("MAE is computed") {
        double mae = Evaluation::computeMAE(users, predictions);
        REQUIRE(mae == Approx(0.5));
    }

    /**
     * @brief Тестирует вычисление RMSE.
     *
     * RMSE = sqrt((5.0 - 4.5)^2) = sqrt(0.25)
     */
    SECTION("RMSE is computed") {
        double rmse = Evaluation::computeRMSE(users, predictions);
        REQUIRE(rmse == Approx(std::sqrt(0.25)));
    }
}
