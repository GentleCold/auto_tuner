add_library(auto_tuner_static STATIC target/cmdline_target.cpp search/grid_search.cpp common.cpp target/target.cpp search/decision_tree_search.cpp)
add_library(auto_tuner_shared SHARED target/cmdline_target.cpp search/grid_search.cpp common.cpp target/target.cpp search/decision_tree_search.cpp)
#install(TARGETS auto_tuner_static DESTINATION ${PROJECT_SOURCE_DIR}/lib/static)
#install(TARGETS auto_tuner_shared DESTINATION ${PROJECT_SOURCE_DIR}/lib/shared)

target_link_libraries(auto_tuner_static armadillo)
target_link_libraries(auto_tuner_shared armadillo)
