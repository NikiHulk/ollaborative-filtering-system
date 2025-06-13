#include <iostream>
#include <filesystem>
#include "DataHandler/CSVLoader.h"
#include "Models/User.h"
#include "Models/Item.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <data_path>" << std::endl;
        return 1;
    }

    //Объявляем DataPAth
    fs::path dataPath(argv[1]);

    std::cout << "Program started, loading data from: "
              << fs::absolute(dataPath) << std::endl;

    if (!fs::exists(dataPath)) {
        std::cerr << "Error: Data file not found at "
                  << fs::absolute(dataPath) << std::endl;
        return 1;
    }

    std::vector<User> users;
    std::vector<Item> items;

    try {
        CSVLoader::load(dataPath.string(), users, items, /*verbose=*/false);
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading data: "
                  << e.what() << std::endl;
        return 1;
    }
    std::cout << "Loaded " << users.size() << " users and "
              << items.size() << " items." << std::endl;
    if (!users.empty()) {
        std::cout << "First user ID: "
                  << users[0].getId()
                  << std::endl;
    }

    std::cout << "Program finished successfully" << std::endl;
    return 0;
}
