#Основная библиотека системы
add_library(RecommenderCore
        DataHandler/CSVLoader.cpp
        DataHandler/DataProcessor.cpp
        Algorithms/SimilarityMetrics.cpp
        Algorithms/CollaborativeFiltering.cpp
        Algorithms/HybridRecommender.cpp
        Models/User.cpp
        Models/Item.cpp
        Models/Rating.cpp
        Utils/Evaluation.cpp
        Utils/TimeAwareness.cpp
)

#Основное приложение
add_executable(recsys main.cpp)
target_link_libraries(recsys RecommenderCore)