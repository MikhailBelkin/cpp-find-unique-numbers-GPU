include(FetchContent)
include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.11)

project(find_unique_numbers  CXX)
set(CMAKE_CXX_STANDARD 14)

#add_subdirectory(./lib/boost)
find_package(OpenCL REQUIRED)
find_package(Boost REQUIRED)


add_executable(find_unique_numbers)
target_sources(find_unique_numbers PRIVATE find_unique_numbers.cpp)
target_include_directories(find_unique_numbers PUBLIC 
			${OpenCL_INCLUDE_DIR}
			${Boost_INCLUDE_DIR}

)
target_link_libraries(find_unique_numbers ${OpenCL_LIBRARY})
target_link_directories(find_unique_numbers PUBLIC ${Boost_LIBRARY})
link_directories(${Boost_LIBRARY})

get_target_property(debug find_unique_numbers INCLUDE_DIRECTORIES)
message(${debug})

