##############
# Unit Tests
##############
enable_testing()

set(PROJECT_TEST_NAME utilsclass_ut_exec)
file(GLOB TEST_SRC_FILES *.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main utilsclass)

INSTALL(TARGETS ${PROJECT_TEST_NAME} DESTINATION ${INSTALL_DIR})
###
## This is so you can do 'make test' to see all your tests run, instead of
## manually running the executable runUnitTests to see those specific tests.
add_test(UnitTests ${PROJECT_TEST_NAME})

if (RUNCOV STREQUAL 1)
    include(CodeCoverage)
    setup_target_for_coverage(travis_gcov_example_coverage ${PROJECT_TEST_NAME} coverage)
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif ()
