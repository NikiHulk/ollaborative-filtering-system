/**
* @file main.cpp
 * @brief Точка входа в программу: запуск системы коллаборативной фильтрации.
 *
 * Загружает пользователей и оценки из CSV-файла, вычисляет рекомендации
 * на основе user-based, item-based и гибридных алгоритмов. Также оценивает
 * точность предсказаний (MAE и RMSE).
 */

#include "Algorithms/Recommender.h"
#include "Algorithms/Evaluation.h"
#include "DataHandler/CSVLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace recsys;

/**
 * @brief Выводит рекомендации на экран.
 *
 * @param title Заголовок для печати (например, "Top-N (user-based):").
 * @param recs Вектор пар <itemId, predictedScore>.
 */

void printRecommendations(const std::string& title, const std::vector<std::pair<int, double>>& recs) {
    std::cout << title << "\n";
    if (recs.empty()) {
        std::cout << "  Нет доступных рекомендаций\n";
    } else {
        for (const auto& [itemId, score] : recs) {
            std::cout << "  Товар #" << itemId << ", предсказанная оценка = "
                      << std::fixed << std::setprecision(3) << score << "\n";
        }
    }
}

/**
 * @brief Основная точка входа в программу.
 *
 * Загружает данные из CSV, выбирает первого пользователя в списке и вычисляет
 * для него рекомендации разными методами.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки. argv[1] — путь к CSV-файлу.
 * @return Код завершения: 0 — успех, 1 — ошибка.
 */

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "==========================================\n";
    std::cout << "     Collaborative Filtering System\n";
    std::cout << "     Система коллаборативных рекомендаций\n";
    std::cout << "==========================================\n\n";

    if (argc < 2) {
        std::cerr << "[ОШИБКА] Использование: recsys <файл_данных.csv>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::cout << "[ЗАГРУЗКА] Чтение данных из: " << filename << "\n";

    std::vector<User> users;
    std::vector<Item> items;
    CSVLoader::load(filename, users, items, true);

    std::cout << "[УСПЕХ] Загружено " << users.size() << " пользователей, " << items.size() << " товаров\n";

    if (users.empty()) {
        std::cerr << "[ОШИБКА] Нет пользователей\n";
        return 1;
    }

    int targetUserId = users.front().getId();
    std::cout << "\n[ЦЕЛЬ] Рекомендации для пользователя ID: #" << targetUserId << "\n";

    const User* user = nullptr;
    for (const auto& u : users) {
        if (u.getId() == targetUserId) {
            user = &u;
            break;
        }
    }

    if (!user) {
        std::cerr << "[ОШИБКА] Пользователь не найден!\n";
        return 1;
    }

    if (user->getRatings().empty()) {
        std::cout << "[COLD START] У пользователя нет оценок. Популярные товары:\n";
        auto popular = Recommender::topPopularItems(items, 3);
        for (auto& [itemId, count] : popular) {
            std::cout << "  Товар #" << itemId << " (оценок: " << count << ")\n";
        }
    } else {
        // --- USER-BASED ---
        auto userRecs = Recommender::recommendTopN(targetUserId, users, items, 3);
        std::cout << "\n[USER-BASED] Рекомендации на основе схожих пользователей:\n";
        printRecommendations("Top-N (user-based):", userRecs);

        std::unordered_map<int, std::unordered_map<int, double>> predictedUser;
        for (const auto& [itemId, rating] : user->getRatings()) {
            double pred = Predictor::predict(targetUserId, itemId, users, 2);
            predictedUser[targetUserId][itemId] = pred;
        }

        std::cout << "\n=== Оценка user-based ===\n";
        std::cout << "MAE  = " << Evaluation::computeMAE(users, predictedUser) << "\n";
        std::cout << "RMSE = " << Evaluation::computeRMSE(users, predictedUser) << "\n";

        // --- ITEM-BASED ---
        auto itemRecs = Recommender::recommendItemBasedTopN(targetUserId, users, items, 3);
        std::cout << "\n[ITEM-BASED] Рекомендации на основе похожих товаров:\n";
        printRecommendations("Top-N (item-based):", itemRecs);

        std::unordered_map<int, std::unordered_map<int, double>> predictedItem;
        for (const auto& [itemId, rating] : user->getRatings()) {
            double pred = Predictor::predictItemBased(targetUserId, itemId, users, items, 2);
            predictedItem[targetUserId][itemId] = pred;
        }

        std::cout << "\n=== Оценка item-based ===\n";
        std::cout << "MAE  = " << Evaluation::computeMAE(users, predictedItem) << "\n";
        std::cout << "RMSE = " << Evaluation::computeRMSE(users, predictedItem) << "\n";

        // --- HYBRID ---
        auto hybridRecs = Recommender::recommendHybrid(targetUserId, users, items, 3, 2, Predictor::Metric::Cosine, 0.5);
        std::cout << "\n[HYBRID] Гибридная рекомендация (50% user + 50% item):\n";
        printRecommendations("Top-N (hybrid):", hybridRecs);

        std::unordered_map<int, std::unordered_map<int, double>> predictedHybrid;
        for (const auto& [itemId, rating] : user->getRatings()) {
            double userPred = Predictor::predict(targetUserId, itemId, users, 2);
            double itemPred = Predictor::predictItemBased(targetUserId, itemId, users, items, 2);
            predictedHybrid[targetUserId][itemId] = 0.5 * userPred + 0.5 * itemPred;
        }

        std::cout << "\n=== Оценка hybrid ===\n";
        std::cout << "MAE  = " << Evaluation::computeMAE(users, predictedHybrid) << "\n";
        std::cout << "RMSE = " << Evaluation::computeRMSE(users, predictedHybrid) << "\n";
    }

    std::cout << "\n[ГОТОВО] Программа завершена успешно.\n";
    return 0;
}
