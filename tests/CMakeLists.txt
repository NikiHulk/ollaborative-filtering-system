add_executable(tests
        test_main.cpp
        test_data_handling.cpp
        test_similarity.cpp
        test_predictor.cpp
        include/catch2/catch_amalgamated.cpp
)

target_link_libraries(tests
        PRIVATE
        RecommenderCore
)

target_include_directories(tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME all_tests COMMAND tests)