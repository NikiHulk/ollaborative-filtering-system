#include "Rating.h"

Rating::Rating(int u, int i, double s, std::time_t t)
    : userId(u), itemId(i), score(s), timestamp(t) {}