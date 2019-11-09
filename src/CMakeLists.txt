
file(GLOB HEADER_LIST "${AutoThrottle_SOURCE_DIR}/include/AutoThrottle/*.h")
file(GLOB SOURCE_LIST "./*.cpp")

# Xplane sdk 
set(
    xplane_sdk_INCLUDE_DIRS
    "${xplane_sdk_SOURCE_DIR}/CHeaders"
    "${xplane_sdk_SOURCE_DIR}/CHeaders/XPLM"
    "${xplane_sdk_SOURCE_DIR}/CHeaders/Widgets"
)

if (WIN32)
    add_compile_definitions(IBM=1)
    set(
        xplane_sdk_LIBS 
        "${xplane_sdk_SOURCE_DIR}/Libraries/Win/XPLM_64.lib"
        "${xplane_sdk_SOURCE_DIR}/Libraries/Win/XPWidgets_64.lib"
    )
elseif(APPLE)
    add_compile_definitions(APL=1)
    set(xplane_sdk_LIBS "${xplane_sdk_SOURCE_DIR}/Libraries/Mac")
else()
    add_compile_definitions(LIN=1)
endif()
add_compile_definitions(XPLM200 XPLM210 XPLM300 XPLM301)

add_library(AutoThrottle SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(AutoThrottle PRIVATE cereal ${xplane_sdk_LIBS})
target_include_directories(AutoThrottle PUBLIC ../include ${xplane_sdk_INCLUDE_DIRS})

set_target_properties(AutoThrottle
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    PREFIX ""
    SUFFIX ".xpl"
)