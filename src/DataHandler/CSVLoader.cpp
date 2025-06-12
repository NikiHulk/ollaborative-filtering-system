#include "CSVLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <ctime>

void CSVLoader::load(const std::string& filename,
                     std::vector<User>& users,
                     std::vector<Item>& items,
                     bool verbose /*= true*/) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    int lineNum = 0;
    int badLines = 0;

    std::unordered_map<int, User*> userMap;
    std::unordered_map<int, Item*> itemMap;

    while (std::getline(file, line)) {
        lineNum++;

        // 1) Убираем всё после '#'
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // 2) Пропуск заголовка и пустых строк
        if (lineNum == 1 || line.empty()) {
            continue;
        }

        // 3) Токенизация + тримминг пробелов
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            auto l = token.find_first_not_of(" \t\r\n");
            auto r = token.find_last_not_of(" \t\r\n");
            if (l != std::string::npos && r != std::string::npos) {
                tokens.push_back(token.substr(l, r - l + 1));
            } else {
                tokens.push_back("");
            }
        }

        // 4) Проверка количества полей
        if (tokens.size() < 3) {
            if (verbose) {
                std::cerr << "Skipping invalid line " << lineNum
                          << ": only " << tokens.size() << " fields\n";
            }
            badLines++;
            continue;
        }

        // 5) Конвертация и валидация
        try {
            int userId    = std::stoi(tokens[0]);
            int itemId    = std::stoi(tokens[1]);
            double rating = std::stod(tokens[2]);

            if (userId <= 0)
                throw std::invalid_argument("Invalid user ID: " + tokens[0]);
            if (itemId <= 0)
                throw std::invalid_argument("Invalid item ID: " + tokens[1]);
            if (rating < 0.0 || rating > 5.0)
                throw std::invalid_argument("Rating out of range: " + tokens[2]);

            std::time_t ts = std::time(nullptr);
            if (tokens.size() > 3 && !tokens[3].empty()) {
                ts = std::stol(tokens[3]);
                if (ts < 0)
                    throw std::invalid_argument("Invalid timestamp: " + tokens[3]);
            }

            // 6) Создаём или находим пользователя
            if (userMap.find(userId) == userMap.end()) {
                users.emplace_back(userId);
                userMap[userId] = &users.back();
                if (verbose) std::cout << "Created user #" << userId << "\n";
            }

            // 7) Создаём или находим предмет
            if (itemMap.find(itemId) == itemMap.end()) {
                items.emplace_back(itemId);
                itemMap[itemId] = &items.back();
                if (verbose) std::cout << "Created item #" << itemId << "\n";
            }

            // 8) Добавляем рейтинг
            Rating r(userId, itemId, rating, ts);
            userMap[userId]->addRating(r);
            itemMap[itemId]->addRating(r);
            if (verbose) {
                std::cout << "Added rating: user=" << userId
                          << ", item=" << itemId
                          << ", rating=" << rating << "\n";
            }
        }
        catch (const std::exception& e) {
            if (verbose) {
                std::cerr << "Error parsing line " << lineNum
                          << ": " << e.what()
                          << "\n  Line content: " << line << "\n";
            }
            badLines++;
        }
    }

    // Итоговая статистика
    if (verbose && badLines > 0) {
        std::cout << "Skipped " << badLines << " invalid lines.\n";
    }
    if (verbose && users.empty()) {
        std::cerr << "Warning: No users loaded!\n";
    }
    if (verbose && items.empty()) {
        std::cerr << "Warning: No items loaded!\n";
    }
}
