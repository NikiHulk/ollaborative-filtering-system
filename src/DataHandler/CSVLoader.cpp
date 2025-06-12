#include "CSVLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unordered_map>


void CSVLoader::load(const std::string& filename,
                    std::vector<User>& users,
                    std::vector<Item>& items) {
    // 1. Открытие файла с дополнительными проверками
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    int lineNum = 0;

    // 2. Используем умные указатели для безопасности
    std::unordered_map<int, User*> userMap;
    std::unordered_map<int, Item*> itemMap;

    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;  // Пропускаем пустые строки
        if (lineNum == 1) continue;  // Пропускаем заголовок

        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // 3. Безопасный парсинг строки
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // 4. Проверка минимального количества полей
        if (tokens.size() < 3) {
            std::cerr << "Skipping invalid line " << lineNum
                      << ": only " << tokens.size() << " fields" << std::endl;
            continue;
        }

        try {
            // 5. Валидация и преобразование данных
            int userId = std::stoi(tokens[0]);
            int itemId = std::stoi(tokens[1]);
            double rating = std::stod(tokens[2]);

            // 6. Проверка диапазонов
            if (userId <= 0) throw std::invalid_argument("Invalid user ID: " + tokens[0]);
            if (itemId <= 0) throw std::invalid_argument("Invalid item ID: " + tokens[1]);
            if (rating < 0 || rating > 5) throw std::invalid_argument("Rating out of range: " + tokens[2]);

            // 7. Обработка timestamp (если есть)
            std::time_t timestamp = std::time(nullptr);
            if (tokens.size() > 3) {
                timestamp = std::stol(tokens[3]);
                if (timestamp < 0) throw std::invalid_argument("Invalid timestamp: " + tokens[3]);
            }

            // 8. Поиск или создание пользователя
            if (userMap.find(userId) == userMap.end()) {
                users.emplace_back(userId);
                userMap[userId] = &users.back();
                std::cout << "Created user #" << userId << std::endl;
            }

            // 9. Поиск или создание товара
            if (itemMap.find(itemId) == itemMap.end()) {
                items.emplace_back(itemId);
                itemMap[itemId] = &items.back();
                std::cout << "Created item #" << itemId << std::endl;
            }

            // 10. Создание и добавление рейтинга
            Rating r(userId, itemId, rating, timestamp);

            // 11. Проверка указателей перед использованием
            if (userMap[userId] == nullptr) {
                throw std::runtime_error("Null user pointer for ID: " + std::to_string(userId));
            }

            if (itemMap[itemId] == nullptr) {
                throw std::runtime_error("Null item pointer for ID: " + std::to_string(itemId));
            }

            userMap[userId]->addRating(r);
            itemMap[itemId]->addRating(r);

            std::cout << "Added rating: user=" << userId << ", item=" << itemId
                      << ", rating=" << rating << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error parsing line " << lineNum << ": " << e.what()
                      << "\nLine: " << line << std::endl;
        }
    }

    // 12. Проверка результатов
    if (users.empty()) {
        std::cerr << "Warning: No users loaded!" << std::endl;
    }

    if (items.empty()) {
        std::cerr << "Warning: No items loaded!" << std::endl;
    }
}