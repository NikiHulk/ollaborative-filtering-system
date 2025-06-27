/**
 * @file RecommenderTest.cpp
 * @brief Тесты для алгоритма Recommender (Top-N рекомендации, популярные объекты и проверка метрик).
 *
 * Проверяется:
 * - генерация рекомендаций на основе похожих пользователей;
 * - исключение уже оценённых объектов;
 * - реакция на несуществующего пользователя;
 * - выдача популярных объектов;
 * - работа с различными метриками схожести.
 */

#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Recommender.h>
#include <Algorithms/Predictor.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>

using namespace recsys;

/**
 * @test Проверяет различные сценарии работы метода Recommender::recommendTopN и topPopularItems.
 */
TEST_CASE("Recommender generates recommendations") {
    std::vector<User> users;
    std::vector<Item> items;

    // User 1 оценивает item 101 и 102
    users.emplace_back(1);
    Rating r1(1, 101, 5.0, 0);
    Rating r2(1, 102, 4.0, 0);
    users[0].addRating(r1);
    users[0].addRating(r2);

    // Добавляем 3 объекта (101 и 102 оценены пользователем 1, 103 — ещё нет)
    items.emplace_back(101);
    items.emplace_back(102);
    items.emplace_back(103);
    items[0].addRating(r1);
    items[1].addRating(r2);
    items[2].addRating(Rating(2, 103, 5.0, 0)); // user 2 оценил 103

    // User 2 — сосед, который оценил 101 и 103
    users.emplace_back(2);
    users[1].addRating(Rating(2, 101, 4.0, 0));
    users[1].addRating(Rating(2, 103, 5.0, 0));

    /**
     * @brief Проверяет, что пользователю предлагаются только те объекты, которые он ещё не оценивал.
     * Ожидается, что 103 будет рекомендован, т.к. он ещё не был оценён user 1.
     */
    SECTION("Returns recommendation for non-rated item") {
        auto recs = Recommender::recommendTopN(1, users, items, 3, 2, Predictor::Metric::Cosine);
        REQUIRE_FALSE(recs.empty());
        REQUIRE(recs[0].first == 103);
        REQUIRE(recs[0].second > 0.0);
    }

    /**
     * @brief Проверяет, что в рекомендациях не содержатся уже оценённые пользователем объекты.
     */
    SECTION("Does not recommend already rated items") {
        auto recs = Recommender::recommendTopN(1, users, items, 5);
        for (const auto& [itemId, _] : recs) {
            REQUIRE(itemId != 101);
            REQUIRE(itemId != 102);
        }
    }

    /**
     * @brief Проверяет, что при попытке получить рекомендации для несуществующего пользователя будет выброшено исключение.
     */
    SECTION("Throws if user not found") {
        REQUIRE_THROWS_AS(
            Recommender::recommendTopN(999, users, items, 3),
            std::runtime_error
        );
    }

    /**
     * @brief Проверяет, что функция topPopularItems возвращает самые часто оцениваемые объекты.
     * Item 101 имеет две оценки и должен быть на первом месте.
     */
    SECTION("Top popular items returns most rated items") {
        // item 103 = 1 рейтинг, 101 = 2, 102 = 1
        auto top = Recommender::topPopularItems(items, 2);
        REQUIRE(top.size() == 2);
        REQUIRE(top[0].first == 101); // наиболее популярный
    }

    /**
     * @brief Проверяет работу Recommender с метрикой Пирсона.
     */
    SECTION("Supports Pearson metric") {
        auto recs = Recommender::recommendTopN(1, users, items, 3, 2, Predictor::Metric::Pearson);
        REQUIRE_FALSE(recs.empty());
        REQUIRE(recs[0].first == 103);
    }
}
