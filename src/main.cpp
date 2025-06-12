#include <iostream>
#include <filesystem>
#include "DataHandler/CSVLoader.h"
#include "Models/User.h"
#include "Models/Item.h"
std::vector<User> users;
std::vector<Item> items;

namespace fs = std::filesystem;

int main() {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data_path>" << std::endl;
        return 1;
    }
    const std::string dataPath = argv[1];

    std::cout << "Program started" << std::endl;

    try {
        // Проверка существования файла
        fs::path dataPath = "data/ratings.csv";
        std::cout << "Looking for data at: " << fs::absolute(dataPath) << std::endl;

        if (!fs::exists(dataPath)) {
            std::cerr << "Error: Data file not found!" << std::endl;
            return 1;
        }

        std::vector<User> users;
        std::vector<Item> items;

        std::cout << "Before CSVLoader" << std::endl;
        CSVLoader::load(dataPath.string(), users, items);
        std::cout << "After CSVLoader" << std::endl;

        // Проверка загруженных данных
        std::cout << "Loaded " << users.size() << " users and "
                  << items.size() << " items" << std::endl;

        if (!users.empty()) {
            std::cout << "First user ID: " << users[0].getId() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        try {
            CSVLoader::load(dataPath.string(), users, items);
            REQUIRE(users.size() == 2); // Добавить после загрузки для проверки размера векторов
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return 1;
        }
        std::cerr << "Unknown exception!" << std::endl;
        return 1;
    }

    std::cout << "Program finished successfully" << std::endl;
    return 0;
}