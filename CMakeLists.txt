cmake_minimum_required(VERSION 3.0.0)
project(AIGC VERSION 0.1.0)

include(CTest)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/AIGC/include
)

enable_testing()

add_executable(AIGC main.cpp)
IF(WIN32)
    target_link_libraries(AIGC libws2_32.a)
ENDIF(WIN32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
