cmake_minimum_required(VERSION 3.0)
project(sh2x)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_C_COMPILER clang)

#set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wold-style-cast -Wextra -Wshadow -Wpointer-arith -Wwrite-strings \
	-Woverloaded-virtual \
	-Wno-uninitialized -Wno-unused-parameter -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
	-Wno-unused-value -march=native -pipe")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}}/lib)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/sh2x SH2X_SRCS)

aux_source_directory(${PROJECT_SOURCE_DIR}/src MAIN_SRCS)

add_executable(main ${MAIN_SRCS} ${SH2X_SRCS})