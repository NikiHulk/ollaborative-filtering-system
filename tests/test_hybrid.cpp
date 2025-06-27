/**
 * @file HybridRecommenderTest.cpp
 * @brief Тест гибридной рекомендательной системы с использованием user-based и item-based подходов.
 */

#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Recommender.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>

using namespace recsys;

/**
 * @test Проверка, что гибридный рекомендатель правильно объединяет предсказания.
 *
 * Создаются два пользователя и три объекта с оценками.
 * Проверяется, что функция `recommendHybrid`:
 * - возвращает непустой список рекомендаций;
 * - включает в него ожидаемый item (103);
 * - возвращает положительный рейтинг.
 */
TEST_CASE("Hybrid recommender blends predictions") {
    std::vector<User> users;
    std::vector<Item> items;

    // Пользователи 1 и 2
    users.emplace_back(1);
    users.emplace_back(2);

    // Оценки: пользователь → item → оценка
    Rating r1(1, 101, 4.0,123456789);
    Rating r2(1, 102, 5.0,123456789);
    Rating r3(2, 101, 4.0,123456789);
    Rating r4(2, 103, 5.0,123456789);

    // Добавляем оценки пользователям
    users[0].addRating(r1);
    users[0].addRating(r2);
    users[1].addRating(r3);
    users[1].addRating(r4);

    // Добавляем объекты
    items.emplace_back(101);
    items.emplace_back(102);
    items.emplace_back(103);

    // Добавляем оценки объектам
    items[0].addRating(r1); items[0].addRating(r3); // item 101
    items[1].addRating(r2);                         // item 102
    items[2].addRating(r4);                         // item 103

    // Получаем рекомендации для пользователя 1
    auto recs = Recommender::recommendHybrid(
        1,               // userId
        users,           // список пользователей
        items,           // список объектов
        2,               // k для user-based
        2,               // k для item-based
        Predictor::Metric::Cosine, // метрика сходства
        0.5              // коэффициент смешивания
    );

    // Проверка: результат не пустой
    REQUIRE_FALSE(recs.empty());

    // Ожидаем, что первой рекомендацией будет item 103
    REQUIRE(recs[0].first == 103);

    // Убедимся, что предсказанный рейтинг положительный
    REQUIRE(recs[0].second > 0.0);
}
