# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(XComponent)

add_subdirectory(libepoxy-1.5.10)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DOHOS_PLATFORM)

include_directories(
    ${NATIVERENDER_ROOT_PATH}
    ${NATIVERENDER_ROOT_PATH}/include
)

add_library(nativenode SHARED
    runner/runner.cpp
    manager/plugin_manager.cpp
    napi_init.cpp
)

find_library(
    # Sets the name of the path variable.
    EGL-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    EGL
)

find_library(
    # Sets the name of the path variable.
    GLES-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    GLESv3
)

find_library(
    # Sets the name of the path variable.
    hilog-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    hilog_ndk.z
)

find_library(
    # Sets the name of the path variable.
    libace-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    ace_ndk.z
)

find_library(
    # Sets the name of the path variable.
    libnapi-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    ace_napi.z
)

find_library(
    # Sets the name of the path variable.
    libuv-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    uv
)

add_subdirectory(VK-GL-CTS)

message(${hilog-lib})
target_link_libraries(nativenode PUBLIC
    ${EGL-lib} ${GLES-lib} ${hilog-lib} ${libace-lib} ${libnapi-lib} ${libuv-lib} librawfile.z.so epoxy)


include_directories(
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/referencerenderer
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/delibs/debase
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/delibs/decpp
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/delibs/dethread
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/common
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/opengl
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/opengl/wrapper
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/framework/qphelper
    ${NATIVERENDER_ROOT_PATH}/VK-Gl-CTS/external/openglcts/modules/runner
    )

target_link_libraries(nativenode PUBLIC
    glcts-package
    glcts-common
    glcts-runner
    tcutil-platform
    )

if (${OHOS_ARCH} STREQUAL "arm64-v8a")
target_link_directories(nativenode PUBLIC ${NATIVERENDER_ROOT_PATH}/../../../libs/${OHOS_ARCH}/)
endif ()