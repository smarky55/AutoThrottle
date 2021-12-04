cmake_minimum_required(VERSION 3.11...3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    AutoThrottle VERSION 0.1.0 
    LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
    cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.0
)
set(JUST_INSTALL_CEREAL ON CACHE BOOL "Don't include cereal tests or sandbox")

FetchContent_Declare(
    xplane_sdk
    URL http://developer.x-plane.com/wp-content/plugins/code-sample-generation/sample_templates/XPSDK301.zip
    URL_HASH SHA256=BF0B38EE82283CE18418CF37AF3ACE83A2CF8E2E88B1951E337BE415961C5CBC
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f67165c2bb2af3060ecae7d20d6f731173485ad0
)
# stb single header libraries
# Latest stb as of 14/12/19

FetchContent_Declare(
    glew
    URL https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download
)

FetchContent_MakeAvailable(cereal xplane_sdk stb glew)

add_subdirectory(src)