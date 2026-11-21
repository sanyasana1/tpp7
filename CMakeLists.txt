cmake_minimum_required(VERSION 3.21)
project(pr_tpp7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Статична бібліотека (варіант 9: сортування масивів + функції лаби №4)
add_library(SortLib STATIC
  src/SortableArray.cpp
  src/SearchUtils.cpp
)
target_include_directories(SortLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Демонстраційна програма
add_executable(SortDemo demo/main.cpp)
target_link_libraries(SortDemo PRIVATE SortLib)

# Демонстрація антипатернів
add_executable(AntipatternDemo antipattern/main.cpp)

# Google Test (vcpkg manifest)
find_package(GTest CONFIG REQUIRED)
enable_testing()

add_executable(SortTests tests/SortTests.cpp)
target_link_libraries(SortTests PRIVATE SortLib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(SortTests)
