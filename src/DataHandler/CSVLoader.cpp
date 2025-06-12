#include "CSVLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void CSVLoader::load(const std::string& filename, 
                    std::vector<User>& users,
                    std::vector<Item>& items) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    int lineNum = 0;
    
    //БЫстрый доступ к пользователям и продукции
    std::unordered_map<int, User*> userMap;
    std::unordered_map<int, Item*> itemMap;

    while (std::getline(file, line)) {
        lineNum++;
        if (lineNum == 1) continue;//Скип заголовка

        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) continue;// Мин размер 3 поля

        try {
            int userId = std::stoi(tokens[0]);
            int itemId = std::stoi(tokens[1]);
            double rating = std::stod(tokens[2]);
            std::time_t timestamp = tokens.size() > 3 ? 
                std::stol(tokens[3]) : std::time(nullptr);

            //Создание пользователя или ищем имеющегося
            if (userMap.find(userId) == userMap.end()) {
                users.emplace_back(userId);
                userMap[userId] = &users.back();
            }

            //Создание продукта или ищем имеющийся
            if (itemMap.find(itemId) == itemMap.end()) {
                items.emplace_back(itemId);
                itemMap[itemId] = &items.back();
            }

            Rating r(userId, itemId, rating, timestamp);
            userMap[userId]->addRating(r);
            itemMap[itemId]->addRating(r);

        } catch (const std::exception& e) {
            //Скип ошибочных строк
            continue;
        }
    }
}