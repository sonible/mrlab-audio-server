# Audio-Server

Server software running on the Audio computer. 





It communicates with the clients, configures the audio devices, and launches the audio apps.


## Building

`mrlabctrl` uses `cmake` for building, so compilation works the standard way, e.g.:

```
mkdir build
cmake -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug
cd build && ninja
```

The project uses [CPM](https://github.com/cpm-cmake/CPM.cmake) for dependency management. It is recommended to specify a cache directory for CPM in order to avoid repeated downloading, e.g.:

```
cmake -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CPM_SOURCE_CACHE=$HOME/.cache/CPM
```

Alternatively, the environment variable `CPM_SOURCE_CACHE` may be set instead of passing the CMake option to achieve the same result.

Note that you might have to explicitly specify the full path for `CPM_SOURCE_CACHE` instead of using `$HOME` in case your IDE does not perform environment variable expansion.
