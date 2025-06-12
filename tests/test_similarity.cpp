// tests/test_similarity.cpp

#include <catch2/catch.hpp>               // Catch2 v3
#include "Algorithms/Similarity.h"        // путь внутри src/
#include "Models/User.h"
#include "Models/Rating.h"

using namespace recsys;

TEST_CASE("Cosine similarity of identical ratings yields 1.0", "[cosine]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 5.0));
    u2.addRating(Rating(2, 101, 5.0));
    REQUIRE( Approx(Similarity::cosine(u1, u2)) == 1.0 );
}

TEST_CASE("Pearson similarity is zero when no common items", "[pearson]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 4.0));
    u2.addRating(Rating(2, 102, 3.0));
    REQUIRE( Similarity::pearson(u1, u2) == 0.0 );
}
