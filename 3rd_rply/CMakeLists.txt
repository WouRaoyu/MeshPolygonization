cmake_minimum_required(VERSION 3.1)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


set(rply_HEADERS
    rply.h
    rplyfile.h
    )

set(rply_SOURCES
    rply.c
    etc/convert.c
    etc/dump.c
    etc/sconvert.c
    )


add_library(${PROJECT_NAME} STATIC ${rply_HEADERS} ${rply_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
                      FOLDER "3rd_party")


target_include_directories(${PROJECT_NAME} PRIVATE
    ${3RD_RPLY_DIR}
    )

target_compile_definitions(${PROJECT_NAME} PRIVATE NO_SIGACTION NO_STRTOK_R _CRT_SECURE_NO_WARNINGS TPI_NONE NPARASCIP WITH_SCIPDEF ROUNDING_FE)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
        )
endif()
