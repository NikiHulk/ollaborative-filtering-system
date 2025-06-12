# src/CMakeLists.txt

add_library(RecommenderCore
        Models/Rating.cpp
        Models/User.cpp
        Models/Item.cpp
        DataHandler/CSVLoader.cpp
)

target_include_directories(RecommenderCore
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(recsys main.cpp)
target_link_libraries(recsys PRIVATE RecommenderCore)
