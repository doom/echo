add_library(echo INTERFACE)

target_sources(echo INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/echo/details/array_view.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/echo/details/string_view_hash.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/echo/debug.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/echo/echo.hpp
        )

target_include_directories(echo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
