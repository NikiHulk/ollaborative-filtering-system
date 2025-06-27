#include <catch2/catch_amalgamated.hpp>
#include <Algorithms/Evaluation.h>
#include <Models/User.h>
#include <Models/Item.h>
#include <DataHandler/CSVLoader.h>

using namespace Catch;

using namespace recsys;
using Catch::Approx;

TEST_CASE("Evaluation metrics compute correctly") {
    std::vector<User> users;

    // user 1: predicted=4.5, actual=5.0
    User u1(1);
    u1.addRating(Rating(1, 101, 5.0, 123456789));
    users.push_back(u1);

    std::unordered_map<int, std::unordered_map<int, double>> predictions = {
        {1, {{101, 4.5}}}
    };

    SECTION("MAE is computed") {
        double mae = Evaluation::computeMAE(users, predictions);
        REQUIRE(mae == Approx(0.5));
    }

    SECTION("RMSE is computed") {
        double rmse = Evaluation::computeRMSE(users, predictions);
        REQUIRE(rmse == Approx(std::sqrt(0.25)));
    }
}
