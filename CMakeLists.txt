cmake_minimum_required(VERSION 3.10)
project(openai-c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB SOURCES
        "src/core.c"
        "src/chat.c"
        "src/image.c"
        "src/audio.c"
        "src/embeddings.c"
)
add_library(openai STATIC ${SOURCES})
target_include_directories(openai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(openai PUBLIC curl cjson)

install(TARGETS openai DESTINATION lib)
install(FILES include/openai.h DESTINATION include)

option(OPENAI_BUILD_EXAMPLES "Build example programs" ON)

if (OPENAI_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_SOURCES examples/*.c)
    foreach(example_file ${EXAMPLE_SOURCES})
        get_filename_component(example_name ${example_file} NAME_WE)
        add_executable(${example_name} ${example_file})
        target_link_libraries(${example_name} openai)
        target_include_directories(${example_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    endforeach()
endif()
