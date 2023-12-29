cmake_minimum_required(VERSION 3.10)
project("GetFeature")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra")

set(PWD_DIR "${PROJECT_SOURCE_DIR}/")
set(ORI_DIR "${PROJECT_SOURCE_DIR}/../..")
include_directories(${ORI_DIR}/3third/include)
include_directories(${PWD_DIR}/source)

#set(OBJECT)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(utils_file ${ORI_DIR}/3third/include/utils.cpp)

if(${PLATFORM} MATCHES "Linux")
    link_directories(${ORI_DIR}/3third/so/${PLATFORM})
    add_executable(main source/main.cpp ${utils_file})
    target_link_libraries(main PUBLIC smart_album pthread dl z)
else()
    link_directories(${ORI_DIR}/3third/so/${PLATFORM}/${ARM_ABI})
    add_executable(main source/main.cpp ${utils_file})
    target_link_libraries(main PUBLIC smart_album pthread dl)
endif()






