# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION "${MIN_VER_CMAKE}" FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE on)
PROJECT(native_object_test)

set(SNPE_INC snpe/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(OpenCV_DIR /usr/local/opencv/share/OpenCV)
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${SNPE_INC}/zdl
        ${OpenCV_INCLUDE_DIRS}
)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DANDROID_TARGET -DSNPE_ENGINE_SUPPORTED -DLOG_SWITCH=0")

add_executable( # Sets the name.
        ${PROJECT_NAME}

        # Sets the library as a shared library.
        #SHARED

        # Provides a relative path to your source file(s).
        ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ObjectDetection.cpp
        )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries(
        # Specifies the target library.
        ${PROJECT_NAME}
        # snpe
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/aarch/libSNPE.so
        # opencv
        ${OpenCV_LIBS}
        # zlib
        z

)