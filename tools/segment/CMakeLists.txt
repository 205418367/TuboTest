cmake_minimum_required(VERSION 3.10)
project("GetFeature")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../")
set(CODE_DIR "${PROJECT_SOURCE_DIR}/")


include_directories(${PLATFORM}_${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SOURCE_DIR}/3third/include>
	$<BUILD_INTERFACE:${CODE_DIR}/src>
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(utils_file ${SOURCE_DIR}/3third/include/utils.cpp)
file(GLOB ENGINE_LIBS ${SOURCE_DIR}/3third/so/${PLATFORM}/${ARM_ABI}/${ANDROID_ABI}/*.so)

if(${PLATFORM} MATCHES "Linux")
    message("#################### ${PLATFORM} ####################")
    add_executable(Linux_main ${CODE_DIR}/src/main.cpp ${utils_file})
    target_link_libraries(Linux_main PUBLIC ${ENGINE_LIBS} dl pthread)
endif()




