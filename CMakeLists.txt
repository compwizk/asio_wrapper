cmake_minimum_required(VERSION 3.5.1)
project(asio)

# Setup color output
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(Reset       "${Esc}[m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
endif()

include(ExternalProject)
if(NOT ASIO_TAG)
    set(ASIO_TAG "asio-1-10-6")
endif()

ExternalProject_Add(
    asio-src
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG "${ASIO_TAG}"
    TMP_DIR "/tmp"
    STAMP_DIR "asio-stamp"
    DOWNLOAD_DIR "asio-src"
    SOURCE_DIR "asio-src"
    BUILD_IN_SOURCE 1
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(ASIO_INCLUDE_PATH "asio-src/asio/include/")





find_package(Threads REQUIRED)

# Create and setup the interface library for asio
add_library(${PROJECT_NAME} INTERFACE)

add_dependencies(${PROJECT_NAME} asio-src)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERFACE_COMPILE_OPTIONS "-std=c++11"
)
target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${ASIO_INCLUDE_PATH}
)
target_link_libraries(${PROJECT_NAME}
    INTERFACE ${CMAKE_THREAD_LIBS_INIT}
)
target_compile_definitions(${PROJECT_NAME}
    INTERFACE ASIO_STANDALONE
    INTERFACE ASIO_HAS_STD_CHRONO
)

# Setup package install
set(INCLUDE_DEST "include")
set(LIB_DEST "lib")
set(EXPORT_DEST "lib/${PROJECT_NAME}")

add_library(${PROJECT_NAME}_EXPORT INTERFACE)
set_target_properties(${PROJECT_NAME}_EXPORT PROPERTIES
    INTERFACE_COMPILE_OPTIONS "-std=c++11"
    EXPORT_NAME ${PROJECT_NAME}
)
target_include_directories(${PROJECT_NAME}_EXPORT INTERFACE
    ${CMAKE_INSTALL_PREFIX}/${INCLUDE_DEST}
)

# Install asio header files
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${ASIO_INCLUDE_PATH}
    DESTINATION ${INCLUDE_DEST}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ${PROJECT_NAME}_EXPORT EXPORT ${PROJECT_NAME} DESTINATION ${LIB_DEST})
install(FILES ${PROJECT_NAME}-config.cmake DESTINATION ${EXPORT_DEST})
install(EXPORT ${PROJECT_NAME} DESTINATION ${EXPORT_DEST})
install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ${CMAKE_INSTALL_PREFIX}/${INCLUDE_DEST}/asio \
    /usr/include/asio)"
)
install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ${CMAKE_INSTALL_PREFIX}/${INCLUDE_DEST}/asio.hpp \
    /usr/include/asio.hpp)"
)
