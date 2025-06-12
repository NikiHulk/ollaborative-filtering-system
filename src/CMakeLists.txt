# Библиотека ядра рекомендаций
add_library(RecommenderCore
        DataHandler/CSVLoader.cpp
        Models/Rating.cpp
        Models/User.cpp
        Models/Item.cpp
)

target_include_directories(RecommenderCore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Приложение
add_executable(recsys main.cpp)
target_link_libraries(recsys PRIVATE RecommenderCore)