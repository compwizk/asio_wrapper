# ASIO Standalone Wrapper

This project takes a specific release version of ASIO standalone and installs it to the base machine.

It supports using the base machines installed headers and installs a library that can be found by CMake. If no package-manager (apt, etc) installed version exists, it will pull a specific tagged version (change the `CMakeLists.txt`) and install the headers and created library.

## Docker

```bash
docker build -t asio:boost -f boostasio.Dockerfile .
docker build -t asio:src -f src.Dockerfile .
docker build -t asio:libasio -f libasio.Dockerfile .
```


## Boost - Asio linking
https://www.boost.org/doc/libs/1_64_0/doc/html/boost_asio/history.html

ONLY FOR LINUX