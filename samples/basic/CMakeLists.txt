cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

set(PROJECT_NAME basic)

project(${PROJECT_NAME} VERSION 0.0.0.1 LANGUAGES CXX)

if(${APPLE})
  message("Apple")
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)
elseif(${MSVC})
  message("MSVC")
  string(REGEX REPLACE "/Z[iI7]" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
  string(REGEX REPLACE "/SAFESEH:(YES|NO)" "" CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
endif()

file(GLOB HDR *.hpp)
file(GLOB SRC *.cpp)

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
#target_include_directories(${PROJECT_NAME_LIB} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME_LIB})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${basic_BINARY_DIR}/assets)
