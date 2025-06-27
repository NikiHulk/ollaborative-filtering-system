#pragma once

#include "../Models/User.h"
#include "../Models/Rating.h"
#include <cmath>
#include <vector>
#include <utility>

namespace recsys {
/**
     * @class Similarity
     * @brief Класс для вычисления метрик схожести в рекомендательных системах
     * 
     * Предоставляет статические методы для расчета различных метрик схожести:
     * - Между пользователями (user-based подход)
     * - Между товарами (item-based подход)
     * - Временное затухание оценок
     */

    ///Статические методы для расчёта метрик сходства пользователей.
    class Similarity {
    public:
/**
         * @brief Вычисляет косинусную схожесть между двумя пользователями
         * 
         * @param u1 Первый пользователь
         * @param u2 Второй пользователь
         * @return double Значение схожести в диапазоне [0, 1]
         */
        ///Косинусная близость
        static double cosine(const User& u1, const User& u2);
/**
         * @brief Вычисляет корреляцию Пирсона между двумя пользователями
         * 
         * @param u1 Первый пользователь
         * @param u2 Второй пользователь
         * @return double Значение корреляции в диапазоне [-1, 1]
         */

        ///Корреляция Пирсона
        static double pearson(const User& u1, const User& u2);
/**
         * @brief Вычисляет коэффициент Жаккара между пользователями
         * 
         * @param u1 Первый пользователь
         * @param u2 Второй пользователь
         * @return double Значение схожести в диапазоне [0, 1]
         */

        ///Жаккард(для бинарных данных)
        static double jaccard(const User& u1, const User& u2);
/**
         * @brief Вычисляет скорректированную косинусную схожесть между товарами
         * 
         * @param users Вектор всех пользователей системы
         * @param itemId1 ID первого товара
         * @param itemId2 ID второго товара
         * @return double Значение схожести в диапазоне [-1, 1]
         */

        static double adjustedCosine(const std::vector<User>& users, int itemId1, int itemId2);
/**
         * @brief Вычисляет схожесть на основе манхэттенского расстояния
         * 
         * @param u1 Первый пользователь
         * @param u2 Второй пользователь
         * @return double Нормализованная схожесть в диапазоне [0, 1]
         */

        static double manhattan(const User& u1, const User& u2);
/**
         * @brief Вычисляет весовой коэффициент с экспоненциальным затуханием
         * 
         * @param timestamp Время события (в секундах с эпохи UNIX)
         * @param now Текущее время (в секундах с эпохи UNIX)
         * @param halfLife Период полураспада (в секундах)
         * @return double Весовой коэффициент в диапазоне (0, 1]
         */

        static double decayWeight(long timestamp, long now, double lambda = 0.01);

    };

}
