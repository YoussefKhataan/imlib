cmake_minimum_required(VERSION 3.15...4.1)

project(
    ImLib 
    VERSION 1.0
    DESCRIPTION "Very nice project"
    LANGUAGES CXX CUDA
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Shared Libraries (.so, .dylib, .dll) go here:
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Static Libraries (.a, .lib) go here:
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
############## MAIN LIBRARY ###############
find_package(CUDAToolkit REQUIRED)

add_library(ImLibLibrary) # <-- remove interface?
target_sources(ImLibLibrary
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/core/data/image.cpp
        ${CMAKE_SOURCE_DIR}/src/core/filter/filters.cpp
        ${CMAKE_SOURCE_DIR}/src/core/filter/filters_cuda.cu
    PUBLIC 
        FILE_SET imlib_headers
            TYPE HEADERS 
            BASE_DIRS ${CMAKE_SOURCE_DIR}/include
            FILES 
                ${CMAKE_SOURCE_DIR}/include/core/global.h
                ${CMAKE_SOURCE_DIR}/include/core/data/image.h
                ${CMAKE_SOURCE_DIR}/include/core/io/image_loader.h
                ${CMAKE_SOURCE_DIR}/include/core/filter/filters.h
)

################## TESTS #####################

find_package(GTest REQUIRED)

add_executable(ImLoaderTest)
target_sources(ImLoaderTest
    PRIVATE
        ${CMAKE_SOURCE_DIR}/test/core/io/im_loader_test.cpp
)
target_link_libraries(ImLoaderTest
    PRIVATE 
        ImLibLibrary
        GTest::gtest
        GTest::gtest_main
)
# target_link_libraries(ImLoaderTest <-- example cuda linkage
#     PRIVATE CUDA::cudart
# )

add_executable(ExportImageTest)
target_sources(ExportImageTest
    PRIVATE
        ${CMAKE_SOURCE_DIR}/test/core/io/export_image_test.cpp
)
target_link_libraries(ExportImageTest
    PRIVATE 
        ImLibLibrary
        GTest::gtest
        GTest::gtest_main
)

file(COPY ${CMAKE_SOURCE_DIR}/test_data 
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
