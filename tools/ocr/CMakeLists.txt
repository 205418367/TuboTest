cmake_minimum_required(VERSION 3.10)
project("GetFeature")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(PWD_DIR "${PROJECT_SOURCE_DIR}/")
set(ORI_DIR "${PROJECT_SOURCE_DIR}/../../")
include_directories(${ORI_DIR}/3third/include)
include_directories(${PWD_DIR}/source)

#set(OBJECT)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
file(GLOB ENGINE_LIBS ${ORI_DIR}/3third/so/${PLATFORM}/*.so)

add_executable(main source/main.cpp )
target_link_libraries(main PUBLIC ${ENGINE_LIBS} pthread dl z)






