cmake_minimum_required(VERSION 3.0)
project(Audio)
set (CMAKE_CXX_STANDARD 20)

file(COPY ${PROJECT_SOURCE_DIR}/res/ 
     DESTINATION ${PROJECT_BINARY_DIR}/res  
)  

add_subdirectory(lib)

add_subdirectory(app)