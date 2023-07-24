cmake_minimum_required(VERSION 3.10)
project("scene")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s -std=c++11") 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -s")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/../..")

include_directories(${PLATFORM}_${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${SOURCE_DIR}/3third/include>
)

set(utils_file ${SOURCE_DIR}/tools/utils.cpp)
file(GLOB ENGINE_LIBS ${SOURCE_DIR}/3third/so/${PLATFORM}/${ANDROID_ABI}/*.so)
if(${PLATFORM} MATCHES "Linux")
    message("#################### ${PLATFORM} ####################")
    add_executable(${PLATFORM}_${PROJECT_NAME} smart_album_img_cls.cpp ${utils_file})
    target_link_libraries(${PLATFORM}_${PROJECT_NAME} PUBLIC
	${ENGINE_LIBS}
	dl pthread
    )
elseif(${PLATFORM} MATCHES "ARM")
    message("#################### ${PLATFORM} ${ANDROID_ABI} ####################")
    if(${ANDROID_ABI} MATCHES "YINGLAINPU")
        set(AQROOT "/home/lichen/desktop/npu/library/acuity-root-dir")
        set(OVXLIB_DIR "${AQROOT}/acuity-ovxlib-dev/arm64")
        link_directories(${OVXLIB_DIR}/lib)
        link_directories(${AQROOT}/lib/arm64)
        add_executable(${ANDROID_ABI}_scene ${SOURCE_DIR}/tools/scene/smart_album_img_cls.cpp ${utils_file})
        target_link_libraries(${ANDROID_ABI}_scene PUBLIC ${ENGINE_LIBS} OpenVX ovxlib VSC GAL ArchModelSw NNArchPerf dl pthread)
    else()
        add_executable(${ANDROID_ABI}_scene smart_album_img_cls.cpp ${utils_file}) 
        target_link_libraries(${ANDROID_ABI}_scene PUBLIC ${ENGINE_LIBS} dl pthread)
    endif()
endif()





