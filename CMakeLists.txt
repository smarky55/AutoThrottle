cmake_minimum_required(VERSION 3.11...3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(AutoThrottle VERSION 0.1.0 
    LANGUAGES CXX)


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

FetchContent_MakeAvailable(cereal xplane_sdk)

add_subdirectory(src)