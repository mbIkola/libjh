cmake_minimum_required(VERSION 3.3)

project(tests)

enable_testing()
include(DownloadProject)

download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)


file(COPY data DESTINATION .)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} gtest libjh)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
