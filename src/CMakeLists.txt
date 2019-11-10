
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
    find_library(XPLM XPLM_64 PATH "${xplane_sdk_SOURCE_DIR}/Libraries/Win")
    find_library(XPWIDGETS XPWidgets_64 PATH "${xplane_sdk_SOURCE_DIR}/Libraries/Win")
elseif(APPLE)
    add_compile_definitions(APL=1)
    find_library(XPLM XPLM PATH "${xplane_sdk_SOURCE_DIR}/Libraries/Mac")
    find_library(XPWIDGETS XPWidgets PATH "${xplane_sdk_SOURCE_DIR}/Libraries/Mac")
else()
    add_compile_definitions(LIN=1)
endif()
add_compile_definitions(XPLM200 XPLM210 XPLM300 XPLM301)

find_package(OpenGL REQUIRED)

add_library(AutoThrottle SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(AutoThrottle PRIVATE cereal ${XPLM} ${XPWIDGETS} OpenGL::GL)
target_include_directories(AutoThrottle PUBLIC ../include ${xplane_sdk_INCLUDE_DIRS} "${OPENGL_INCLUDE_DIR}")

set_target_properties(AutoThrottle
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    CXX_STANDARD 17
    PREFIX ""
    SUFFIX ".xpl"
)