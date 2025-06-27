#pragma once

#include <unordered_map>
#include <utility>
#include <functional>

/**
 * @struct pair_hash
 * @brief Хеш-функция для std::pair, используемая в std::unordered_map.
 *
 * Эта структура позволяет использовать пары (например, std::pair<int, int>)
 * в качестве ключей в std::unordered_map. Она определяет пользовательскую
 * функцию хеширования, которая комбинирует хеши первого и второго элементов пары.
 *
 * Пример использования:
 * @code
 * std::unordered_map<std::pair<int, int>, double, pair_hash> cache;
 * cache[{1, 2}] = 3.14;
 * @endcode
 */
struct pair_hash {
    /**
     * @brief Функтор для вычисления хеша от пары значений.
     *
     * @tparam T1 Тип первого элемента пары.
     * @tparam T2 Тип второго элемента пары.
     * @param p Пара, для которой нужно вычислить хеш.
     * @return Хеш-значение типа std::size_t.
     */
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);  ///< Хеш первого элемента
        auto h2 = std::hash<T2>{}(p.second); ///< Хеш второго элемента
        return h1 ^ (h2 << 1); ///< Комбинация хешей (XOR + сдвиг)
    }
};
