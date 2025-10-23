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
cmake -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CPM_SOURCE_CACHE=$HOME/.cache/CPM -D CPM_USE_NAMED_CACHE_DIRECTORIES=1
```

Note that you might have to explicitly specify the full path for `CPM_SOURCE_CACHE` instead of using `$HOME` in case your IDE does not perform environment variable expansion.  The option `CPM_USE_NAMED_CACHE_DIRECTORIES` makes the dependency directories look better in IDEs like CLion.

Both options can also be set via environment variables of the same names (i.e., `CPM_SOURCE_CACHE`).
