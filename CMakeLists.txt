cmake_minimum_required(VERSION 3.12)

project(path_planning LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(a_star src/a_star.cpp)

# find_package(behaviortree_cpp REQUIRED)

# add_executable(basic1 src/basic1.cpp)
# add_executable(basic2 src/basic2.cpp)
# add_executable(basic3 src/basic3.cpp)

# target_include_directories(basic1 PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(basic1 BT::behaviortree_cpp)
#
# target_include_directories(basic2 PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(basic2 BT::behaviortree_cpp)
#
# target_include_directories(basic3 PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(basic3 BT::behaviortree_cpp)
