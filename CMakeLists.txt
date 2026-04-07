cmake_minimum_required(VERSION 4.1)
set (gameName "Game")
project(${gameName})

set(LIBRARIES
    SDL3
    SDL3_image
    SDL3_ttf
    tinyxml2
)
set(LINK_TARGETS "")
set(DIRECTORIES
    assets
    maps
)

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
    PRIVATE src include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${LINK_TARGETS}
)
foreach(directory ${DIRECTORIES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${directory}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${directory}
    )
endforeach()