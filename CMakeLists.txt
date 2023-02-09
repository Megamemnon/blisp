cmake_minimum_required(VERSION 3.0.0)
project(blisp VERSION 0.1.0)

include(CTest)
enable_testing()

set(SOURCES main.c io.c lexparse.c interpreter.c)
add_executable(blisp ${SOURCES})

set(CMAKE_C_FLAGS -lreadline)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
