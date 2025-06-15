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
                     bool verbose) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open file: " + filename);

    std::string line;
    int lineNum = 0, badLines = 0;

    // Вместо User* и Item* храним индекс в векторе:
    std::unordered_map<int, size_t> userIndex;
    std::unordered_map<int, size_t> itemIndex;

    while (std::getline(file, line)) {
        ++lineNum;
        if (lineNum == 1 || line.empty()) continue;

        std::istringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(ss, token, ',')) {
            auto l = token.find_first_not_of(" \t\r\n");
            auto r = token.find_last_not_of(" \t\r\n");
            tokens.push_back(
                (l != std::string::npos)
                ? token.substr(l, r - l + 1)
                : ""
            );
        }
        if (tokens.size() < 3) { ++badLines; continue; }

        try {
            int userId    = std::stoi(tokens[0]);
            int itemId    = std::stoi(tokens[1]);
            double rating = std::stod(tokens[2]);
            std::time_t ts = std::time(nullptr);
            if (tokens.size() > 3 && !tokens[3].empty()) {
                ts = std::stol(tokens[3]);
            }
            if (!userIndex.count(userId)) {
                users.emplace_back(userId);
                userIndex[userId] = users.size() - 1;
                if (verbose) std::cout << "Created user #" << userId << "\n";
            }
            if (!itemIndex.count(itemId)) {
                items.emplace_back(itemId);
                itemIndex[itemId] = items.size() - 1;
                if (verbose) std::cout << "Created item #" << itemId << "\n";
            }
            auto& u = users[userIndex[userId]];
            auto& it = items[itemIndex[itemId]];
            Rating r(userId, itemId, rating, ts);
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

    if (verbose && badLines > 0)
        std::cout << "Skipped " << badLines << " invalid lines.\n";
}
