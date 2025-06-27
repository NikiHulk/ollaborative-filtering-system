#include "Rating.h"

/**
 * @brief Конструктор класса Rating.
 * @param u ID пользователя.
 * @param i ID товара.
 * @param s Значение оценки.
 * @param t Временная метка выставления оценки.
 */
Rating::Rating(int u, int i, double s, std::time_t t)
    : userId(u), itemId(i), score(s), timestamp(t) {}
