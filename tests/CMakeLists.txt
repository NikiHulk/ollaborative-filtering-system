# tests/CMakeLists.txt

add_executable(tests
        test_data_handling.cpp
        test_similarity.cpp
)

# Линкуем с ядром и с Catch2::Catch2WithMain
target_link_libraries(tests
        PRIVATE
        RecommenderCore
        Catch2::Catch2WithMain
)

# Добавляем include-путь к src, чтобы тесты видели ваши заголовки
target_include_directories(tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
