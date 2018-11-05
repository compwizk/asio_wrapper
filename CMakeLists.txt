cmake_minimum_required(VERSION 3.5.1)
project(asio)

if(WIN32)
    message(FATAL_ERROR "Unsupported host operating system detected!")
endif()

set(INSTALL_WRAPPER_HEADER ON CACHE BOOL 
    "Install the wrapper header file to support unified boost and asio-src interface.")

# Setup color output
string(ASCII 27 Esc)
set(Reset       "${Esc}[m")
set(Green       "${Esc}[32m")
set(Yellow      "${Esc}[33m")

# Check if libasio-dev or asio-src was installed
find_path(ASIO_INCLUDE_PATH asio.hpp HINTS 
    "/usr/include"
    "/usr/local/include"
    "/opt/local/include"
)
if(ASIO_INCLUDE_PATH)
    message(STATUS "${Yellow}Found existing ASIO install${Reset}")
    set(LIBASIO_INSTALL TRUE)
else(ASIO_INCLUDE_PATH)
    message(STATUS "${Green}No existing ASIO-src install found${Reset}")
endif(ASIO_INCLUDE_PATH)

# Check if libboost was installed and no libasio-dev
if(NOT LIBASIO_INSTALL)
    find_package(Boost COMPONENTS system QUIET)
    if(Boost_FOUND)
        message(STATUS "${Yellow}Found existing Boost::ASIO install${Reset}")
        message(STATUS "${Yellow}>> You can use cmake find_package(Boost) to access asio.${Reset}")
    else(Boost_FOUND)
        message(STATUS "${Green}No existing Boost::ASIO install found${Reset}")
    endif(Boost_FOUND)
endif(NOT LIBASIO_INSTALL)

# If no libboost or libasio-dev, default to src install
if(NOT Boost_FOUND AND NOT LIBASIO_INSTALL)
    set(SRC_INSTALL TRUE)
endif(NOT Boost_FOUND AND NOT LIBASIO_INSTALL)


if(NOT Boost_FOUND)
    # Asio depends on Threads (asio can run without this, but not recommended)
    find_package(Threads REQUIRED)
    # Create and setup the interface library for asio
    add_library(${PROJECT_NAME} INTERFACE)
    # Setup package install
    set(INCLUDE_DEST "include")
    set(LIB_DEST "lib")
    set(EXPORT_DEST "lib/${PROJECT_NAME}")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERFACE_COMPILE_OPTIONS "-std=c++11"
        EXPORT_NAME ${PROJECT_NAME}
    )
    target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${ASIO_INCLUDE_PATH}>
        $<INSTALL_INTERFACE:${INCLUDE_DEST}>
    )
    target_link_libraries(${PROJECT_NAME}
        INTERFACE ${CMAKE_THREAD_LIBS_INIT}
    )
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE ASIO_STANDALONE
        INTERFACE ASIO_HAS_STD_CHRONO
    )
endif(NOT Boost_FOUND)


if(SRC_INSTALL)
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
    add_dependencies(${PROJECT_NAME} asio-src)

    # Install asio header files
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${ASIO_INCLUDE_PATH}
        DESTINATION ${INCLUDE_DEST}
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.ipp"
    )

    # Symlink /usr/local install to /usr
    install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink \
        ${CMAKE_INSTALL_PREFIX}/${INCLUDE_DEST}/asio \
        /usr/include/asio)"
    )
    install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink \
        ${CMAKE_INSTALL_PREFIX}/${INCLUDE_DEST}/asio.hpp \
        /usr/include/asio.hpp)"
    )

endif(SRC_INSTALL)

if(LIBASIO_INSTALL OR SRC_INSTALL)
    # Install target and asio cmake files for find_package
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION ${LIB_DEST})
    install(FILES ${PROJECT_NAME}-config.cmake DESTINATION ${EXPORT_DEST})
    install(EXPORT ${PROJECT_NAME} DESTINATION ${EXPORT_DEST})
endif(LIBASIO_INSTALL OR SRC_INSTALL)

if(INSTALL_WRAPPER_HEADER)
    if(NOT INCLUDE_DEST)
        set(INCLUDE_DEST "include")
    endif()
    # Install asio wrapper header file
    install(
        FILES "${CMAKE_SOURCE_DIR}/asio_wrapper.hpp"
        DESTINATION ${INCLUDE_DEST}
    )
endif()
