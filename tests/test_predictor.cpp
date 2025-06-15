#include <catch2/catch_amalgamated.hpp>

using namespace Catch;

#include <vector>
#include "../src/Models/User.h"
#include "../src/Models/Rating.h"
#include "../src/Algorithms/Similarity.h"
#include "../src/Algorithms/Predictor.h"

using namespace recsys;

TEST_CASE("Predictor returns 0 when no neighbors rated", "[predictor]") {
    User u1(1), u2(2);
    // только u1 ─ целевой, u2 не ставил itemId=101
    u1.addRating(Rating(1, 101, 4.0));
    std::vector<User> users = {u1, u2};

    REQUIRE(Predictor::predict(1, 101, users, 1) == Approx( /* decide semantics */ 0.0 ));
}

TEST_CASE("Predictor uses one neighbor", "[predictor]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 4.0));
    u2.addRating(Rating(2, 101, 5.0));
    std::vector<User> users = {u1, u2};

    // косинус будет =1, значит предсказанная = 5.0
    REQUIRE(Predictor::predict(1, 101, users, 1) == Approx(5.0));
}

TEST_CASE("Predictor uses Pearson similarity", "[predictor][pearson]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 4.0));
    u2.addRating(Rating(2, 101, 5.0));
    std::vector<User> users = {u1,u2};
    REQUIRE(
      Approx(Predictor::predict(1,101,users,1, Predictor::Metric::Pearson))
      == 5.0
    );
}

TEST_CASE("Predictor uses Jaccard similarity", "[predictor][jaccard]") {
    User u1(1), u2(2);
    // Jaccard для одного пересечения = 1, рейтинг берётся у соседа
    u1.addRating(Rating(1, 101, 0.0));
    u2.addRating(Rating(2, 101, 3.0));
    std::vector<User> users = {u1,u2};
    REQUIRE(
      Approx(Predictor::predict(1,101,users,1, Predictor::Metric::Jaccard))
      == 3.0
    );
}