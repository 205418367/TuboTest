cmake_minimum_required(VERSION 3.10)
project("GetFeature")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../")
set(CODE_DIR "${PROJECT_SOURCE_DIR}/")

#set(THIRDPARTY_PATH "${CODE_DIR}/3third/TI-Thirdparty")
#include(${THIRDPARTY_PATH}/CMakeLists.txt)

include_directories(${PLATFORM}_${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SOURCE_DIR}/3third/include>
	$<BUILD_INTERFACE:${CODE_DIR}/src>
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(utils_file ${SOURCE_DIR}/3third/include/utils.cpp)
file(GLOB ENGINE_LIBS ${SOURCE_DIR}/3third/so/${PLATFORM}/${ARM_ABI}/${ANDROID_ABI}/*.so)

if(${PLATFORM} MATCHES "Linux")
    message("#################### ${PLATFORM} ####################")
    # export img embedding
    add_executable(Linux_imgJson ${CODE_DIR}/src/export_imgJson.cpp ${utils_file})
    target_link_libraries(Linux_imgJson PUBLIC ${ENGINE_LIBS} ${OpenCV_LIBS} z dl pthread)
    # export txt embedding
    add_executable(Linux_appear ${CODE_DIR}/src/appearChange.cpp ${utils_file})
    target_link_libraries(Linux_appear PUBLIC ${ENGINE_LIBS} dl pthread)

elseif(${PLATFORM} MATCHES "ARM" OR ${PLATFORM} MATCHES "Android")
    message("#################### ${PLATFORM} ${ANDROID_ABI} ####################")
    if(${ANDROID_ABI} MATCHES "YINGLAINPU")
        set(AQROOT "/home/lichen/desktop/npu/library/acuity-root-dir")
        set(OVXLIB_DIR "${AQROOT}/acuity-ovxlib-dev/arm64")
        link_directories(${OVXLIB_DIR}/lib)
        link_directories(${AQROOT}/lib/arm64)
        add_executable(${ANDROID_ABI}_cluster ${SOURCE_DIR}/tools/face/test_cluster.cpp ${utils_file})
        target_link_libraries(${ANDROID_ABI}_cluster PUBLIC ${ENGINE_LIBS} OpenVX ovxlib VSC GAL ArchModelSw NNArchPerf dl pthread)
    else()
        # export embedding
        #add_executable(Android_embedding ${CODE_DIR}/src/export_imgfeat.cpp ${utils_file})
        #target_link_libraries(Android_embedding PUBLIC ${ENGINE_LIBS} dl pthread)
        # test platform
        add_executable(Android_platform ${CODE_DIR}/src/test_platform.cpp ${utils_file})
        target_link_libraries(Android_platform PUBLIC ${ENGINE_LIBS} dl)
        # test search
        #add_executable(Android_search ${CODE_DIR}/src/test_search.cpp ${utils_file})
        #target_link_libraries(Android_search PUBLIC ${ENGINE_LIBS} dl pthread)
    endif()
endif()




