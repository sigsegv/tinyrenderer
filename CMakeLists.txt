cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

set(PROJECT_NAME_LIB tiny)

project(${PROJECT_NAME_LIB} VERSION 0.0.0.1 LANGUAGES CXX)

include(local_settings.cmake OPTIONAL)

if(${APPLE})
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)
endif()

file(GLOB HDR include/*.hpp;include/*.h)
file(GLOB SRC src/*.cpp)

add_library(${PROJECT_NAME_LIB} STATIC ${SRC} ${HDR})
target_include_directories(${PROJECT_NAME_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

if(${MSVC})
  set(CWD_INTERMEDIATE "")
else()
  set(CWD_INTERMEDIATE "/${CMAKE_CFG_INTDIR}")
endif()

#add_custom_command(TARGET ${PROJECT_NAME_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}${CWD_INTERMEDIATE}/assets)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
add_subdirectory("samples/basic")
add_subdirectory("samples/sfml")