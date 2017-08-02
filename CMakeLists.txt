cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

set(PROJECT_NAME tiny)

project(${PROJECT_NAME} VERSION 0.0.0.1 LANGUAGES CXX)

include(local_settings.cmake OPTIONAL)

if(${APPLE})
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)
endif()

file(GLOB HDR *.hpp;*.h)
file(GLOB SRC *.cpp)

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/assets)
