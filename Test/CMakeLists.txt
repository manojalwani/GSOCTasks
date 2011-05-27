project(gsoc)
cmake_minimum_required(VERSION 2.6)

find_package(GDCM REQUIRED)
include(${GDCM_USE_FILE})

add_executable(justforfun test1.cxx)
target_link_libraries(justforfun gdcmMSFF)


