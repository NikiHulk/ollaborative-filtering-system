#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Recommender.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>

using namespace recsys;

TEST_CASE("Hybrid recommender blends predictions") {
    std::vector<User> users;
    std::vector<Item> items;

    users.emplace_back(1);
    users.emplace_back(2);

    Rating r1(1, 101, 4.0);
    Rating r2(1, 102, 5.0);
    Rating r3(2, 101, 4.0);
    Rating r4(2, 103, 5.0);

    users[0].addRating(r1);
    users[0].addRating(r2);
    users[1].addRating(r3);
    users[1].addRating(r4);

    items.emplace_back(101);
    items.emplace_back(102);
    items.emplace_back(103);

    items[0].addRating(r1); items[0].addRating(r3);
    items[1].addRating(r2);
    items[2].addRating(r4);

    auto recs = Recommender::recommendHybrid(1, users, items, 2, 2, Predictor::Metric::Cosine, 0.5);

    REQUIRE_FALSE(recs.empty());
    REQUIRE(recs[0].first == 103);
    REQUIRE(recs[0].second > 0.0);
}
