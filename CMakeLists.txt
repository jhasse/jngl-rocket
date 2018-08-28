add_subdirectory("subprojects/jngl")
add_subdirectory("subprojects/libRocket/Build")

add_executable(jngl-rocket src/main.cpp)
target_link_libraries(jngl-rocket jngl RocketCore)
target_include_directories(jngl-rocket PRIVATE subprojects/libRocket/Include)
