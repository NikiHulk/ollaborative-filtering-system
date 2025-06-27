#include <catch2/catch_amalgamated.hpp>

using namespace Catch;

#include <vector>
#include "../src/Models/User.h"
#include "../src/Models/Rating.h"
#include "../src/Algorithms/Similarity.h"
#include "../src/Algorithms/Predictor.h"

using namespace recsys;

TEST_CASE("Predictor uses one neighbor") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 0});
    u1.addRating({1, 102, 4});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 5});
    u2.addRating({2, 102, 4});
    users.push_back(u2);

    double predicted = Predictor::predict(1, 101, users, 1);
    REQUIRE(predicted == Approx(5.0).margin(0.01));
}

TEST_CASE("Predictor uses Pearson similarity") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 2});
    u1.addRating({1, 102, 4});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 4});
    u2.addRating({2, 102, 6});
    users.push_back(u2);

    double pred = Predictor::predict(1, 101, users, 1, Predictor::Metric::Pearson);
    REQUIRE(pred == Approx(4.0).margin(0.01));
}

TEST_CASE("Predictor uses Jaccard similarity") {
    std::vector<User> users;

    User u1(1);
    u1.addRating({1, 101, 3.0});
    u1.addRating({1, 102, 3.0});
    users.push_back(u1);

    User u2(2);
    u2.addRating({2, 101, 3.0});
    u2.addRating({2, 102, 3.0});
    users.push_back(u2);

    double pred = Predictor::predict(1, 101, users, 1, Predictor::Metric::Jaccard);
    REQUIRE( pred == Approx(5.0).margin(0.01) );

}