cmake_minimum_required(VERSION 3.5.1)
project(sample_asio)

# Asio
find_package(asio QUIET)
if(NOT asio_FOUND)
    find_package(Boost REQUIRED system)
    set(asio_LIBRARIES ${Boost_LIBRARIES})
    message(STATUS "Using Boost::ASIO - ${asio_LIBRARIES}")
else()
    set(asio_LIBRARIES asio)
    message(STATUS "Using Standalone::ASIO - ${asio_LIBRARIES}")
endif()

# Build the executable
add_executable(${PROJECT_NAME} async_tcp_echo_server.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${asio_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
