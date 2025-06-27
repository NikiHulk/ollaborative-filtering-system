/**
 * @file SimilarityTest.cpp
 * @brief Тесты для методов вычисления сходства между пользователями и объектами.
 *
 * Включает проверку:
 * - косинусного сходства (Cosine)
 * - коэффициента корреляции Пирсона (Pearson)
 * - меры Жаккара (Jaccard)
 * - скорректированного косинусного сходства (Adjusted Cosine)
 */

#include <catch2/catch_amalgamated.hpp>

using namespace Catch;

#include <Algorithms/Similarity.h>
#include <Models/User.h>
#include <Models/Rating.h>

using namespace recsys;

/**
 * @test Проверяет, что косинусное сходство двух пользователей,
 * поставивших одинаковую единственную оценку одному и тому же объекту, равно 1.
 */
TEST_CASE("Cosine of identical single-item ratings is 1", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 5.0,0));
    u2.addRating(Rating(2, 101, 5.0,0));
    REQUIRE(Similarity::cosine(u1, u2) == Approx(1.0));
}

/**
 * @test Проверяет, что коэффициент Пирсона равен 0, если у пользователей нет общих оценённых объектов.
 */
TEST_CASE("Pearson zero when no overlap", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 5.0, 0));
    u2.addRating(Rating(2, 102, 3.0,0));
    REQUIRE(Similarity::pearson(u1, u2) == Approx(0.0));
}

/**
 * @test Проверяет, что мера Жаккара равна 0, если у пользователей нет пересечений по оценённым объектам.
 */
TEST_CASE("Jaccard of non-overlapping sets is 0", "[similarity]") {
    User u1(1), u2(2);
    u1.addRating(Rating(1, 101, 1.0, 0));
    u2.addRating(Rating(2, 102, 1.0, 0));
    REQUIRE(Similarity::jaccard(u1, u2) == Approx(0.0));
}

/**
 * @test Проверяет скорректированное косинусное сходство между двумя объектами.
 *
 * Пользователи выставили оценки так, что нормализованные векторы идеально совпадают.
 * Ожидаемое значение: 1.0
 */
TEST_CASE("Adjusted Cosine Similarity works - Positive correlation gives +1") {
    User u1(1), u2(2);
    u1.addRating({1, 101, 4.0, 0});
    u1.addRating({1, 102, 2.0, 0});
    u2.addRating({2, 101, 5.0, 0});
    u2.addRating({2, 102, 3.0, 0});

    std::vector<User> users = {u1, u2};
    double sim = Similarity::adjustedCosine(users, 101, 102);
    REQUIRE(sim == Approx(1.0).margin(0.01));
}
