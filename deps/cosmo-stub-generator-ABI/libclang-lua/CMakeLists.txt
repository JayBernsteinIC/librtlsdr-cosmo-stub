cmake_minimum_required(VERSION 3.18)
project(libclang-lua CXX)

find_path(CLANG_INCLUDE_DIR clang-c/CXFile.h
          HINTS /usr/lib/llvm-20/include
                /usr/local/include
                /usr/include)
message(STATUS "CLANG_INCLUDE_DIR = ${CLANG_INCLUDE_DIR}")
if (CLANG_INCLUDE_DIR)
  include_directories(${CLANG_INCLUDE_DIR})
endif()
find_package(Lua REQUIRED)
find_library(CLANG_LIBRARY NAMES clang libclang
             HINTS /usr/lib /usr/lib/llvm-20/lib /usr/local/lib)
find_library(CLANG_CPP_LIBRARY NAMES clang-cpp libclang-cpp
             HINTS /usr/lib /usr/lib/llvm-20/lib /usr/local/lib)
             

if(APPLE)
   set(CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS
       "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -flat_namespace -undefined dynamic_lookup")
endif()

include_directories(${LUA_INCLUDE_DIR})
add_library(libclang-lua MODULE libclang-lua.cpp)
target_link_libraries(libclang-lua ${CLANG_LIBRARY} ${CLANG_CPP_LIBRARY})
set_target_properties(libclang-lua PROPERTIES PREFIX "")

