# ASIO Standalone Wrapper

This project takes a specific release version of ASIO standalone and installs it to the base machine.

It supports using the base machines installed headers and installs a library that can be found by CMake. If no package-manager (apt, etc) installed version exists, it will pull a specific tagged version (change the `CMakeLists.txt`) and install the headers and created library.

