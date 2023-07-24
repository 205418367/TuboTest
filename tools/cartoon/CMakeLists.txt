cmake_minimum_required(VERSION 3.10)
project("cartoon")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -fpermissive")

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../")
include_directories(${PLATFORM}_${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SOURCE_DIR}/include>
)

set(utils_file ${SOURCE_DIR}/src/utils.cpp)
if(${PLATFORM} MATCHES "Linux")
    message("#################### ${PLATFORM} ####################")
    file(GLOB ENGINE_LIBS ${SOURCE_DIR}/so/${PLATFORM}/*.so)
elseif(${PLATFORM} MATCHES "ARM")
    message("#################### ${PLATFORM} ${ARM_ABI} ####################")
    file(GLOB ENGINE_LIBS ${SOURCE_DIR}/so/${PLATFORM}/${ARM_ABI}/*.so)
endif()

add_executable(${PLATFORM}_${PROJECT_NAME} test_Cartoon.cpp ${utils_file})
target_link_libraries(${PLATFORM}_${PROJECT_NAME} PUBLIC
	${ENGINE_LIBS}
	dl pthread
)



