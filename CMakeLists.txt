cmake_minimum_required(VERSION 3.16)
project(akuma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
    )
elseif(PkgConfig_FOUND)
    target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS}
    )
else()
    target_link_libraries(${PROJECT_NAME}
        SDL2 SDL2_image SDL2_ttf
    )
endif()
