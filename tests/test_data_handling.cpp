/**
 * @file test_csvloader.cpp
 * @brief Модульные тесты для загрузчика CSV-файлов (CSVLoader).
 *
 * Этот файл проверяет:
 * - Корректную загрузку пользователей и товаров из CSV.
 * - Обработку ошибок при отсутствии файла.
 * - Игнорирование некорректных строк.
 */

#include <catch2/catch_amalgamated.hpp>
using namespace Catch;

#include <vector>
#include <fstream>
#include <DataHandler/CSVLoader.h>
#include <Models/User.h>
#include <Models/Item.h>

/**
 * @brief Группа тестов: Загрузка данных из CSV
 */
TEST_CASE("CSVLoader loads data correctly") {

    std::vector<User> users;
    std::vector<Item> items;

    // Создание тестового CSV-файла
    std::ofstream testFile("test_data.csv");
    testFile << "userId,itemId,rating,timestamp\n";
    testFile << "1,101,5.0,1672531200\n";
    testFile << "1,102,3.5,1672617600\n";
    testFile << "2,101,4.5,1672704000\n";
    testFile.close();

    /**
     * @section Проверка: Корректная загрузка CSV
     * Ожидаем 2 пользователя, 2 товара, и корректные оценки.
     */
    SECTION("Basic loading") {
        CSVLoader::load("test_data.csv", users, items);

        REQUIRE(users.size() == 2);  ///< Должно быть 2 пользователя
        REQUIRE(items.size() == 2);  ///< Должно быть 2 товара

        auto const& user1 = users[0];
        REQUIRE(user1.getRatingForItem(101) == 5.0);
        REQUIRE(user1.getRatingForItem(102) == 3.5);

        auto const& item101 = items[0];
        REQUIRE(item101.getAverageRating() == Approx((5.0 + 4.5) / 2.0)); ///< Среднее по item 101
    }

    /**
     * @section Проверка: Ошибка при отсутствии файла
     */
    SECTION("Non-existent file throws") {
        std::vector<User> u2;
        std::vector<Item> i2;

        REQUIRE_THROWS_AS(
            CSVLoader::load("no_such_file.csv", u2, i2),
            std::runtime_error
        );
    }

    /**
     * @section Проверка: Некорректные строки игнорируются
     * В файле есть одна плохая строка и одна хорошая.
     * Ожидаем одного пользователя и один товар.
     */
    SECTION("Malformed lines are skipped") {
        std::ofstream bad("bad.csv");
        bad << "userId,itemId,rating\n";
        bad << "x,y,z\n";          // некорректная строка
        bad << "3,103,4.0\n";      // корректная строка
        bad.close();

        std::vector<User> u3;
        std::vector<Item> i3;
        CSVLoader::load("bad.csv", u3, i3);

        REQUIRE(u3.size() == 1);
        REQUIRE(i3.size() == 1);
    }
}
