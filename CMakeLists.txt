cmake_minimum_required(VERSION 3.1)

get_filename_component(SOLUTION_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${SOLUTION_NAME})

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(CGAL REQUIRED COMPONENTS Core)

set(POLYFIT_scip_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_scip)
set(POLYFIT_soplex_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_soplex)
set(3RD_RPLY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_rply)

add_subdirectory(3rd_scip)
add_subdirectory(3rd_soplex)
add_subdirectory(3rd_rply)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # 因为头文件中包含了子目录
include_directories(${POLYFIT_scip_DIR})
include_directories(${3RD_RPLY_DIR})

file(GLOB source "MeshPolygonization/*.h" "MeshPolygonization/*.cpp")
add_executable(${SOLUTION_NAME} ${source})
target_link_libraries(${SOLUTION_NAME} CGAL::CGAL 3rd_scip 3rd_soplex 3rd_rply)