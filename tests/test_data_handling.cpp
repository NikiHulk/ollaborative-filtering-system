#include <catch2/catch.hpp>
#include "../src/DataHandler/CSVLoader.h"
#include "../src/Models/User.h"
#include "../src/Models/Item.h"

TEST_CASE("CSVLoader loads data correctly") {
    std::vector<User> users;
    std::vector<Item> items;
    
    //Создаем временный цсв
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
        
        //Проверка пользователя_1
        auto& user1 = users[0];
        REQUIRE(user1.getRatingForItem(101) == 5.0);
        REQUIRE(user1.getRatingForItem(102) == 3.5);
        
        //Проверка товара_101
        auto& item101 = items[0];
        REQUIRE(item101.getAverageRating() == (5.0 + 4.5) / 2);
    }
    
    SECTION("Invalid file handling") {
        REQUIRE_THROWS_AS(
            CSVLoader::load("non_existent.csv", users, items),
            std::runtime_error
        );
    }
    
    SECTION("Malformed lines skipping") {
        std::ofstream badFile("bad_data.csv");
        badFile << "userId,itemId,rating\n";
        badFile << "1,101,5.0\n";
        badFile << "invalid,data,here\n";
        badFile << "2,102,4.0\n";
        badFile.close();
        
        CSVLoader::load("bad_data.csv", users, items);
        REQUIRE(users.size() == 2);
    }
}