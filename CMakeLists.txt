cmake_minimum_required(VERSION 3.6)
project(jngl-rocket)

add_subdirectory("subprojects/jngl")
add_subdirectory("subprojects/RmlUi")

file(GLOB SRC src/*.cpp)
add_executable(jngl-rocket ${SRC})

target_link_libraries(jngl-rocket jngl RmlCore)
target_include_directories(jngl-rocket PRIVATE subprojects/RmlUi/Include)

if(NOT BUILD_SHARED_LIBS)
	target_compile_definitions(jngl-rocket PRIVATE RMLUI_STATIC_LIB)
endif()
