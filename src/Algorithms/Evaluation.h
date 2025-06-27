#pragma once
#include <vector>
#include <unordered_map>
#include <Models/User.h>

namespace recsys {

    class Evaluation {
    public:
        static double computeMAE(const std::vector<User>& users,
                                 const std::unordered_map<int, std::unordered_map<int, double>>& predicted);

        static double computeRMSE(const std::vector<User>& users,
                                  const std::unordered_map<int, std::unordered_map<int, double>>& predicted);
    };

}
