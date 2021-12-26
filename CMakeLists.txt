cmake_minimum_required (VERSION 3.5)

project(lazyfoo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") 
set(source_dir "${POJECT_SRC_DIR}./src/")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB source_files "${source_dir}/*.cpp")

add_executable(lazyfoo ${source_files})

message(${SDL2_LIBRARIES})

target_link_libraries(lazyfoo ${SDL2_LIBRARIES} SDL2::Main SDL2::Image SDL2::TTF)
#target_link_libraries(lazyfoo ${SDL2_LIBRARIES})
