/**
 * @file CSVLoader.cpp
 * @brief Загрузка данных из CSV-файла в память (пользователи, товары, оценки).
 */

#include "CSVLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <ctime>

namespace recsys {

/**
 * @brief Загружает пользователей, товары и оценки из CSV-файла.
 *
 * CSV-файл должен иметь минимум 3 колонки: `userId,itemId,rating` и опционально `timestamp`.
 * Строка заголовков (первая строка) автоматически пропускается.
 *
 * @param filename Путь к CSV-файлу.
 * @param users Вектор, куда будут добавлены считанные пользователи.
 * @param items Вектор, куда будут добавлены считанные товары.
 * @param verbose Если true, печатает процесс загрузки (создание объектов и ошибки).
 *
 * @throws std::runtime_error если файл не может быть открыт.
 */
void CSVLoader::load(const std::string& filename,
                     std::vector<User>& users,
                     std::vector<Item>& items,
                     bool verbose) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Cannot open file: " + filename);

    std::string line;
    int lineNum = 0;     ///< Номер текущей строки
    int badLines = 0;    ///< Количество некорректных строк

    // Карты для отслеживания индексов пользователей и товаров в векторах
    std::unordered_map<int, size_t> userIndex;
    std::unordered_map<int, size_t> itemIndex;

    while (std::getline(file, line)) {
        ++lineNum;
        if (lineNum == 1 || line.empty()) continue; // пропуск заголовка

        // Разделение строки по запятой
        std::istringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(ss, token, ',')) {
            auto l = token.find_first_not_of(" \t\r\n");
            auto r = token.find_last_not_of(" \t\r\n");
            tokens.push_back((l != std::string::npos) ? token.substr(l, r - l + 1) : "");
        }

        if (tokens.size() < 3) {
            ++badLines;
            continue;
        }

        try {
            int userId    = std::stoi(tokens[0]);       ///< ID пользователя
            int itemId    = std::stoi(tokens[1]);       ///< ID товара
            double rating = std::stod(tokens[2]);       ///< Оценка
            long timestamp = std::time(nullptr);        ///< Временная метка

            // Если в CSV есть четвертая колонка — используем её как timestamp
            if (tokens.size() >= 4 && !tokens[3].empty()) {
                timestamp = std::stol(tokens[3]);
            }

            // Добавление пользователя, если новый
            if (!userIndex.count(userId)) {
                users.emplace_back(userId);
                userIndex[userId] = users.size() - 1;
                if (verbose) std::cout << "Created user #" << userId << "\n";
            }

            // Добавление товара, если новый
            if (!itemIndex.count(itemId)) {
                items.emplace_back(itemId);
                itemIndex[itemId] = items.size() - 1;
                if (verbose) std::cout << "Created item #" << itemId << "\n";
            }

            // Добавление оценки
            auto& u = users[userIndex[userId]];
            auto& it = items[itemIndex[itemId]];
            Rating r(userId, itemId, rating, timestamp);
            u.addRating(r);
            it.addRating(r);

            if (verbose) {
                std::cout << "Added rating: user=" << userId
                          << ", item=" << itemId
                          << ", rating=" << rating << "\n";
            }
        }
        catch (const std::exception& e) {
            ++badLines;
            if (verbose) {
                std::cerr << "Error parsing line " << lineNum
                          << ": " << e.what() << "\n";
            }
        }
    }

    if (verbose && badLines > 0) {
        std::cout << "Skipped " << badLines << " invalid lines.\n";
    }
}

}
