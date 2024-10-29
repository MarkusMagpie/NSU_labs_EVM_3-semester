cmake_minimum_required(VERSION 3.20)

project(lab6)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(Findlibusb-1.0)

find_package(libusb-1.0 REQUIRED)

add_executable(lab5 main.cpp)
target_include_directories(lab5 PUBLIC ${LIBUSB_1_INCLUDE_DIRS})
target_link_libraries(lab5 ${LIBUSB_1_LIBRARIES})
