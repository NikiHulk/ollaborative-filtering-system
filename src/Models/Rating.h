#pragma once
#include <chrono>

struct Rating {
    int userId;
    int itemId;
    double score;
    std::time_t timestamp; //время

    Rating(int u, int i, double s, std::time_t t = std::time(nullptr))
        : userId(u), itemId(i), score(s), timestamp(t) {}
};
