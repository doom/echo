add_executable(test
        ${CMAKE_CURRENT_SOURCE_DIR}/int_pair.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vec_2d.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nested.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

set(ECHOED_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/int_pair.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vec_2d.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nested.hpp
        )

add_custom_target(
        echo_generation
        COMMAND
        ${CMAKE_SOURCE_DIR}/bin/echogen
        ${ECHOED_SRC}
        -extra-arg=-DECHOGEN_GENERATION_PASS -extra-arg=-I${CMAKE_SOURCE_DIR}/library/include -extra-arg=-std=c++17 --
)

add_dependencies(echo_generation echogen)
add_dependencies(test echo_generation)

target_link_libraries(test PRIVATE echo)
