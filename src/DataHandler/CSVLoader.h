#pragma once

#include <string>
#include <vector>
#include "../Models/User.h"
#include "../Models/Item.h"

class CSVLoader {
public:
    // Обновлённая сигнатура: в конце добавлен параметр verbose с дефолтным значением
    static void load(const std::string& filename,
                     std::vector<User>& users,
                     std::vector<Item>& items,
                     bool verbose = true);
};
