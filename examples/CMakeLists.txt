set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) # EXE

add_library(auto_tuner_shared_release SHARED IMPORTED)
set_target_properties(auto_tuner_shared_release PROPERTIES IMPORTED_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libauto_tuner_shared.so)

add_executable(example matrix_multiplication/example.cpp)
target_link_libraries(example auto_tuner_shared_release)

add_executable(example2 function_test/example2.cpp)
target_link_libraries(example2 auto_tuner_shared_release)