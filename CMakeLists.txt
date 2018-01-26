cmake_minimum_required(VERSION 3.4)
project(database)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(database
    util.cpp
    fd_algorithm.cpp
    lossless_decomposition.cpp
    normal_form.cpp
)

add_executable(main 
    main.cpp
)

target_link_libraries(main
    database
)

enable_testing()

add_executable(test_main
    test_util.cpp
    test_lossless_decomposition.cpp
    test_fd_algorithm.cpp
    test_normal_form.cpp
    test_problem.cpp
)

find_library(GMOCK_MAIN gmock_main HINTS lib/test)

target_link_libraries(test_main
    database
    ${GMOCK_MAIN}
    pthread
)

add_test(NAME TestMain COMMAND test_main)
