#pragma once
#include <string>
#include <vector>
#include "../Models/User.h"
#include "../Models/Item.h"

class CSVLoader {
public:
    /**
     * @brief Загружает данные из CSV файла
     * 
     * @param filename Путь к CSV файлу
     * @param users [out] Загруженные пользователи
     * @param items [out] Загруженные товары
     * 
     * @throws std::runtime_error Если файл не может быть открыт
     * 
     * Формат CSV: userId,itemId,rating[,timestamp]
     */
    static void load(const std::string& filename, 
                    std::vector<User>& users,
                    std::vector<Item>& items);
};