project(gsoc)
cmake_minimum_required(VERSION 2.6)

find_package(GDCM REQUIRED)
include(${GDCM_USE_FILE})

add_executable(justforfun Extracting_lowest_resolution.cxx)
target_link_libraries(justforfun gdcmMSFF openjpeg)


