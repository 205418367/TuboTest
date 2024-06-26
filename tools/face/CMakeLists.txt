cmake_minimum_required(VERSION 3.10)
project("GetFeature")
message("==============================>")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s -std=c++17") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../")
set(CODE_DIR "${PROJECT_SOURCE_DIR}/")


include_directories(${PLATFORM}_${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SOURCE_DIR}/3third/include>
	$<BUILD_INTERFACE:${CODE_DIR}/src>
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(utils_file ${SOURCE_DIR}/3third/include/utils.cpp)
file(GLOB ENGINE_LIBS ${SOURCE_DIR}/3third/so/${PLATFORM}/${ARM_ABI}/*.so)

if(${PLATFORM} MATCHES "Linux")
    message("#################### ${PLATFORM} ####################")
    add_executable(Linux_featsimilar ${CODE_DIR}/src/test_featsimilar.cpp ${utils_file})
    target_link_libraries(Linux_featsimilar PUBLIC ${ENGINE_LIBS} z m dl pthread)

    add_executable(Linux_cluster ${CODE_DIR}/src/smart_album_face_cluster.cpp ${utils_file})
    target_link_libraries(Linux_cluster PUBLIC ${ENGINE_LIBS} z m dl pthread)
elseif(${PLATFORM} MATCHES "ARM" OR ${PLATFORM} MATCHES "Android")
    message("#################### ${PLATFORM} ${ANDROID_ABI} ####################")
    if(${ANDROID_ABI} MATCHES "YINGLAINPU")
        set(AQROOT "/home/lichen/desktop/npu/library/acuity-root-dir")
        set(OVXLIB_DIR "${AQROOT}/acuity-ovxlib-dev/arm64")
        link_directories(${OVXLIB_DIR}/lib)
        link_directories(${AQROOT}/lib/arm64)
        add_executable(${ANDROID_ABI}_cluster ${SOURCE_DIR}/tools/face/test_similar.cpp ${utils_file})
        target_link_libraries(${ANDROID_ABI}_cluster PUBLIC ${ENGINE_LIBS} OpenVX ovxlib VSC GAL ArchModelSw NNArchPerf dl pthread)
    else()
        add_executable(arm_thread ${CODE_DIR}/src/test_cluster.cpp ${utils_file})
        target_link_libraries(arm_thread PUBLIC ${ENGINE_LIBS} dl)
    endif()
endif()




