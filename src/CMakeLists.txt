
file(GLOB_RECURSE HEADER_LIST "${AutoThrottle_SOURCE_DIR}/include/AutoThrottle/*.h")
file(GLOB_RECURSE SOURCE_LIST "./*.cpp")

source_group(TREE "${AutoThrottle_SOURCE_DIR}/include/AutoThrottle" PREFIX "Headers" FILES ${HEADER_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source" FILES ${SOURCE_LIST})

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

find_package(OpenGL)
if (NOT OpenGL_FOUND AND NOT WIN32)
    message(STATUS "Fetching GL/gl.h from mesa library")
    execute_process(COMMAND "sudo" "apt-get" "install" "-y" "mesa-common-dev" "libgl1-mesa-dev")
    find_package(OpenGL REQUIRED)
endif()

find_library(GLEW glew32s PATH "${glew_SOURCE_DIR}/lib/Release/x64")

add_library(AutoThrottle SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(AutoThrottle PRIVATE cereal ${XPLM} ${XPWIDGETS} ${GLEW} OpenGL::GL)
target_include_directories(AutoThrottle PUBLIC ../include ${xplane_sdk_INCLUDE_DIRS} "${OPENGL_INCLUDE_DIR}" "${stb_SOURCE_DIR}" "${glew_SOURCE_DIR}/include")

set_target_properties(AutoThrottle
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/$<$<CONFIG:Debug>:>"
    CXX_STANDARD 17
    PREFIX ""
    SUFFIX ".xpl"
)
