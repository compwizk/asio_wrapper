# ASIO Standalone Wrapper

This project addresses an inconvenience with the `libasio-dev` system package or standalone header-only installation from the asio github where pkg-config (\*.pc) nor cmake find (\*.cmake) are created rendering CMake PkgConfig and find_package unusable. We provide a CMake file that you can use to:

- Patch your existing `libasio-dev` inststance to work with find_package
- Install a specific version of asio (pass `ASIO_TAG` with the desired version)
	- If you need to maintain boost version parity, check [here](https://www.boost.org/doc/libs/1_64_0/doc/html/boost_asio/history.html) to find your appropriate version
- If boost is found, leave system untouched


## Docker

A set of dockerfiles that were used to test the different base sytem configuration scenarios is provided. The build commands are:

```bash
docker build -t asio:boost -f boostasio.Dockerfile .
docker build -t asio:src -f src.Dockerfile .
docker build -t asio:libasio -f libasio.Dockerfile .
```

To evaluate the different scenarios, you can simply mount the `asio_wrapper` directory into a container and run `cmake ..`, `make`, and `make install` from a build directory.
