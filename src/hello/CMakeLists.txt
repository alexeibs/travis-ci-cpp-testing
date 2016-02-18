cmake_minimum_required(VERSION 3.4)
project(hello)

include(ExternalProject)

message("my vars: $ENV{MY_CMAKE} $ENV{MY_CMAKE_GEN}")

## GoogleTest/GoogleMock
ExternalProject_Add(gtest
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  GIT_REPOSITORY https://github.com/google/googletest
  CMAKE_COMMAND $ENV{MY_CMAKE}
  CMAKE_GENERATOR $ENV{MY_CMAKE_GEN}
  CMAKE_ARGS -Dgtest_disable_pthreads:BOOL=ON
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
add_dependencies(libgmock gtest)

set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
)
include_directories("${source_dir}/googletest/include")

set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
)
include_directories("${source_dir}/googlemock/include")

## CryptoPP
ExternalProject_Add(cryptopp
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/cryptopp
  GIT_REPOSITORY https://github.com/weidai11/cryptopp
  INSTALL_COMMAND ""
  CMAKE_ARGS -DBUILD_TESTING:BOOL=OFF
)
ExternalProject_Get_Property(cryptopp source_dir binary_dir)

add_library(libcryptopp IMPORTED STATIC GLOBAL)
add_dependencies(libcryptopp cryptopp)

set_target_properties(libcryptopp PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/libcryptopp.a"
)
include_directories("${source_dir}/..")


aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  libgtest
  libgmock
  libcryptopp
)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
