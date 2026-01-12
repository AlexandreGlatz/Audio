cmake_minimum_required(VERSION 3.0)

project(Audio_app)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)
    
add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Audio::library
)