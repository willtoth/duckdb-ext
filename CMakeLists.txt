cmake_minimum_required(VERSION 3.22)

# Set extension name here
set(TARGET_NAME wpilog)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DuckDB's extension distribution supports vcpkg. As such, dependencies can be added in ./vcpkg.json and then
# used in cmake with find_package. Feel free to remove or replace with other dependencies.
# Note that it should also be removed from vcpkg.json to prevent needlessly installing it..
find_package(OpenSSL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    allwpilib
    URL https://github.com/wpilibsuite/allwpilib/archive/refs/tags/v2025.1.1-beta-1.tar.gz
    URL_HASH MD5=657cc8ec15f14737283dd502ba82f8ce
)

set(BUILD_SHARED_LIBS ON)
set(WITH_CSCORE OFF)
set(WITH_EXAMPLES OFF)
set(WITH_GUI OFF)
set(WITH_JAVA OFF)
set(WITH_NTCORE OFF)
set(WITH_SIMULATION_MODULES OFF)
set(WITH_TESTS OFF)
set(WITH_WPILIB OFF)
set(WITH_WPIMATH OFF)
set(WITH_PROTOBUF OFF)
set(WITH_WPIUNITS OFF)
set(NO_WERROR ON)
set(USE_SYSTEM_FMTLIB ON)

FetchContent_MakeAvailable(allwpilib)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)

project(${TARGET_NAME})
include_directories(src/include)

set(EXTENSION_SOURCES src/wpilog_extension.cpp)

build_static_extension(${TARGET_NAME} ${EXTENSION_SOURCES})
build_loadable_extension(${TARGET_NAME} " " ${EXTENSION_SOURCES})

# Link OpenSSL in both the static library as the loadable extension
target_link_libraries(${EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto wpiutil)
target_link_libraries(${LOADABLE_EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto wpiutil)

install(
    TARGETS ${EXTENSION_NAME}
    EXPORT "${DUCKDB_EXPORT_SET}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
