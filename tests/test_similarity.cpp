#include <catch2/catch_amalgamated.hpp>

using namespace Catch;

#include <Algorithms/Similarity.h>
#include <Models/User.h>
#include <Models/Rating.h>

using namespace recsys;

TEST_CASE("Cosine of identical single-item ratings is 1", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 5.0));
    u2.addRating(Rating(2, 101, 5.0));
    REQUIRE( Approx(Similarity::cosine(u1, u2)) == 1.0 );
}

TEST_CASE("Pearson zero when no overlap", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 5.0));
    u2.addRating(Rating(2, 102, 3.0));
    REQUIRE( Approx(Similarity::pearson(u1, u2)) == 0.0 );
}

TEST_CASE("Jaccard of non-overlapping sets is 0", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 1.0));
    u2.addRating(Rating(2, 102, 1.0));
    REQUIRE( Approx(Similarity::jaccard(u1, u2)) == 0.0 );
}
