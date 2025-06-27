#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Recommender.h>
#include <Algorithms/Predictor.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>


using namespace recsys;

TEST_CASE("Recommender generates recommendations") {
    std::vector<User> users;
    std::vector<Item> items;

    // User 1 оценивает 101 и 102
    users.emplace_back(1);
    Rating r1(1, 101, 5.0, 0);
    Rating r2(1, 102, 4.0, 0);
    users[0].addRating(r1);
    users[0].addRating(r2);

    items.emplace_back(101);
    items.emplace_back(102);
    items.emplace_back(103); // Неоценённый item
    items[0].addRating(r1);
    items[1].addRating(r2);
    items[2].addRating(Rating(2, 103, 5.0, 0));

    // User 2 — сосед
    users.emplace_back(2);
    users[1].addRating(Rating(2, 101, 4.0, 0));
    users[1].addRating(Rating(2, 103, 5.0, 0));

    SECTION("Returns recommendation for non-rated item") {
        auto recs = Recommender::recommendTopN(1, users, items, 3, 2, Predictor::Metric::Cosine);
        REQUIRE_FALSE(recs.empty());
        REQUIRE(recs[0].first == 103);
        REQUIRE(recs[0].second > 0.0);
    }

    SECTION("Does not recommend already rated items") {
        auto recs = Recommender::recommendTopN(1, users, items, 5);
        for (const auto& [itemId, _] : recs) {
            REQUIRE(itemId != 101);
            REQUIRE(itemId != 102);
        }
    }

    SECTION("Throws if user not found") {
        REQUIRE_THROWS_AS(
            Recommender::recommendTopN(999, users, items, 3),
            std::runtime_error
        );
    }

    SECTION("Top popular items returns most rated items") {
        // item 103 = 1 rating, 101 = 2, 102 = 1
        auto top = Recommender::topPopularItems(items, 2);
        REQUIRE(top.size() == 2);
        REQUIRE(top[0].first == 101); // должен быть первым (2 оценки)
    }

    SECTION("Supports Pearson metric") {
        auto recs = Recommender::recommendTopN(1, users, items, 3, 2, Predictor::Metric::Pearson);
        REQUIRE_FALSE(recs.empty());
        REQUIRE(recs[0].first == 103);
    }
}
