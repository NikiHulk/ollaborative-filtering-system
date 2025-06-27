/**
* @file CSVLoader.h
 * @brief Заголовочный файл для класса CSVLoader, реализующего загрузку данных из CSV-файла.
 */

#pragma once

#include <string>
#include <vector>
#include "../Models/User.h"
#include "../Models/Item.h"

namespace recsys {

    /**
     * @class CSVLoader
     * @brief Класс для загрузки пользователей, товаров и оценок из CSV-файла.
     *
     * CSV-файл должен содержать записи вида:
     * ```
     * userId,itemId,rating[,timestamp]
     * ```
     * Где:
     * - `userId` — целочисленный идентификатор пользователя.
     * - `itemId` — целочисленный идентификатор товара.
     * - `rating` — дробная оценка от пользователя товару.
     * - `timestamp` *(необязательно)* — метка времени (в формате `time_t`).
     *
     * Класс заполняет векторы `User` и `Item`, создавая нужные связи между ними.
     */
    class CSVLoader {
    public:
        /**
         * @brief Загружает пользователей, товары и оценки из CSV-файла.
         *
         * @param filename Путь к CSV-файлу.
         * @param users Вектор, куда будут добавлены уникальные пользователи (по `userId`).
         * @param items Вектор, куда будут добавлены уникальные товары (по `itemId`).
         * @param verbose Если `true`, выводит дополнительную информацию о процессе загрузки.
         *
         * @note Первая строка CSV-файла считается заголовком и пропускается.
         * @note В случае ошибок в строках (например, некорректные числа) строка пропускается, но программа продолжается.
         * @throw std::runtime_error Если файл не может быть открыт.
         */
        static void load(const std::string& filename,
                         std::vector<User>& users,
                         std::vector<Item>& items,
                         bool verbose = true);
    };

} // namespace recsys
