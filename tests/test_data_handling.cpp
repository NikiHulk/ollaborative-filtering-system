#include <catch2/catch_amalgamated.hpp>
using namespace Catch;

#include <vector>
#include <fstream>

#include <DataHandler/CSVLoader.h>
#include <Models/User.h>
#include <Models/Item.h>

TEST_CASE("CSVLoader loads data correctly") {

    std::vector<User> users;
    std::vector<Item> items;

    std::ofstream testFile("test_data.csv");
    testFile << "userId,itemId,rating,timestamp\n";
    testFile << "1,101,5.0,1672531200\n";
    testFile << "1,102,3.5,1672617600\n";
    testFile << "2,101,4.5,1672704000\n";
    testFile.close();

    SECTION("Basic loading") {
        CSVLoader::load("test_data.csv", users, items);
        REQUIRE(users.size() == 2);
        REQUIRE(items.size() == 2);

        auto const& user1 = users[0];
        REQUIRE(user1.getRatingForItem(101) == 5.0);
        REQUIRE(user1.getRatingForItem(102) == 3.5);

        auto const& item101 = items[0];
        REQUIRE(item101.getAverageRating()
                == Approx((5.0 + 4.5)/2.0));
    }

    SECTION("Non-existent file throws") {
        std::vector<User> u2;
        std::vector<Item> i2;
        REQUIRE_THROWS_AS(
            CSVLoader::load("no_such_file.csv", u2, i2),
            std::runtime_error
        );
    }

    SECTION("Malformed lines are skipped") {
        std::ofstream bad("bad.csv");
        bad << "userId,itemId,rating\n";
        bad << "x,y,z\n";
        bad << "3,103,4.0\n";
        bad.close();

        std::vector<User> u3;
        std::vector<Item> i3;
        CSVLoader::load("bad.csv", u3, i3);
        REQUIRE(u3.size() == 1);
        REQUIRE(i3.size() == 1);
    }
}
