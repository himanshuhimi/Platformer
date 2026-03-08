cmake_minimum_required(VERSION 3.23)
project(Platformer)

set(LIBRARIES
    SDL3
    SDL3_image
    tinyxml2
)
set(LINK_TARGETS "")

foreach(lib ${LIBRARIES})
    find_package(${lib} REQUIRED)
    list(APPEND LINK_TARGETS ${lib}::${lib})
endforeach()

file(GLOB_RECURSE PRIVATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/private/*.cpp
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${PRIVATE_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${LINK_TARGETS}
)