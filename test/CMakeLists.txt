add_subdirectory(lib/googletest)
#include_directories(lib/googletest/googletest/include)

enable_testing()

set(SOURCE_FILES main.cpp src/search_tests.cpp src/cmdline_target_tests.cpp)
add_executable(test_all ${SOURCE_FILES})

target_link_libraries(test_all auto_tunner_shared GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_all)