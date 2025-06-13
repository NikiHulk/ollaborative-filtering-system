#pragma once
#include <ctime>

struct Rating {
    int userId;
    int itemId;
    double score;
    std::time_t timestamp;

    Rating(int u, int i, double s, std::time_t t = std::time(nullptr));
    Rating() = default;
};