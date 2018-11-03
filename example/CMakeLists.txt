cmake_minimum_required(VERSION 3.5.1)
project(sample_asio)

# Verifies asio is installed
find_package(asio REQUIRED)

# Set the library dependencies
set(LIB_DEPENDENCIES asio)

# Build the executable
add_executable(${PROJECT_NAME} async_tcp_echo_server.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_DEPENDENCIES})
