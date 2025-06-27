/**
 * @file PredictorTest.cpp
 * @brief Тесты для проверки работы предсказателя оценок на основе разных метрик схожести.
 *
 * Проверяются предсказания по одному соседу и с использованием различных метрик:
 * - Косинусное сходство (по умолчанию)
 * - Сходство Пирсона
 * - Мера Жаккара
 */

#include <catch2/catch_amalgamated.hpp>

using namespace Catch;

#include <vector>
#include "../src/Models/User.h"
#include "../src/Models/Rating.h"
#include "../src/Algorithms/Similarity.h"
#include "../src/Algorithms/Predictor.h"

using namespace recsys;

/**
 * @test Проверяет, что предсказание строится корректно на основе одного соседа.
 *
 * Пользователь 1 не оценивал item 101, но пользователь 2 — оценивал его на 5.
 * Ожидаемое предсказание: 5.0
 */
TEST_CASE("Predictor uses one neighbor") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 0,0});   // рейтинг 0 — неизвестен
    u1.addRating({1, 102, 4,0});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 5,0});
    u2.addRating({2, 102, 4,0});
    users.push_back(u2);

    double predicted = Predictor::predict(1, 101, users, 1);
    REQUIRE(predicted == Approx(5.0).margin(0.01));
}

/**
 * @test Проверяет, что используется метрика Пирсона при предсказании.
 *
 * Рейтинги подобраны так, что между пользователями наблюдается линейная зависимость.
 * Ожидаемое предсказание: 4.0 (перенос значений по корреляции)
 */
TEST_CASE("Predictor uses Pearson similarity") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 2,0});
    u1.addRating({1, 102, 4,0});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 4,0});
    u2.addRating({2, 102, 6,0});
    users.push_back(u2);

    double pred = Predictor::predict(1, 101, users, 1, Predictor::Metric::Pearson);
    REQUIRE(pred == Approx(4.0).margin(0.01));
}

/**
 * @test Проверяет предсказание на основе меры Жаккара.
 *
 * Пользователи имеют одинаковые множества оценённых объектов.
 * Ожидаемое предсказание: 5.0 (при максимальном сходстве)
 */
TEST_CASE("Predictor uses Jaccard similarity") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 3.0, 123456789});
    u1.addRating({1, 102, 3.0, 123456789});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 3.0, 123456789});
    u2.addRating({2, 102, 3.0, 123456789});
    users.push_back(u2);

    double pred = Predictor::predict(1, 101, users, 1, Predictor::Metric::Jaccard);
    REQUIRE(pred == Approx(5.0).margin(0.01));
}
