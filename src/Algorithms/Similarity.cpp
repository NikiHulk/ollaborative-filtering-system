#include "Algorithms/Similarity.h"

namespace recsys {

    double Similarity::cosine(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();

        double dot = 0.0, norm1 = 0.0, norm2 = 0.0;
        for (auto& [item, rating] : r1) {
            norm1 += rating.score * rating.score;
            auto it = r2.find(item);
            if (it != r2.end()) {
                dot += rating.score * it->second.score;
            }
        }
        for (auto& [item, rating] : r2) {
            norm2 += rating.score * rating.score;
        }
        if (norm1 == 0.0 || norm2 == 0.0) return 0.0;
        return dot / (std::sqrt(norm1) * std::sqrt(norm2));
    }

    double Similarity::pearson(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();

        std::vector<std::pair<double,double>> common;
        for (auto& [item, rating1] : r1) {
            auto it = r2.find(item);
            if (it != r2.end()) {
                common.emplace_back(rating1.score, it->second.score);
            }
        }
        int n = common.size();
        if (n == 0) return 0.0;

        double sum1=0, sum2=0, sum1Sq=0, sum2Sq=0, pSum=0;
        for (auto& [x,y] : common) {
            sum1   += x;
            sum2   += y;
            sum1Sq += x*x;
            sum2Sq += y*y;
            pSum   += x*y;
        }
        double num = pSum - (sum1 * sum2 / n);
        double den = std::sqrt((sum1Sq - sum1*sum1/n) * (sum2Sq - sum2*sum2/n));
        return (den == 0.0) ? 0.0 : num/den;
    }

    double Similarity::jaccard(const User& u1, const User& u2) {
        const auto& r1 = u1.getRatings();
        const auto& r2 = u2.getRatings();
        int inter = 0;
        for (auto& [item, _] : r1) {
            if (r2.count(item)) inter++;
        }
        int uni = r1.size() + r2.size() - inter;
        return uni == 0 ? 0.0 : static_cast<double>(inter)/uni;
    }

}
