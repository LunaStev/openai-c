cmake_minimum_required(VERSION 3.10)
project(openai-c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(openai STATIC src/openai.c)
target_include_directories(openai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(openai PUBLIC curl cjson)

install(TARGETS openai DESTINATION lib)
install(FILES include/openai.h DESTINATION include)

option(OPENAI_BUILD_EXAMPLES "Build example programs" ON)

if (OPENAI_BUILD_EXAMPLES)
    add_executable(openai_example examples/chat.c)
    target_link_libraries(openai_example openai)
endif()
