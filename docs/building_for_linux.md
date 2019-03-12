# Building for Linux

- [Requirements](#requirements)
- [Locations and Environment Variables](#locations-and-environment-variables)
- [Building](#building)
- [Contributing](#contributing)

## Requirements

The following packages are necessary for compiling: 

`sudo apt install build-essential qtbase5-dev qttools5-dev qtdeclarative5-dev qtmultimedia5-dev qt5-default qttools5-dev-tools`

If you install Qt 5.12 you should have the above packages.

Along with either `clang` or `g++`. At least `g++-7` is required, due to C++17 support.

For linting with `clang-tidy` the following are necessary:

`sudo apt install bear clang-tidy`

All applications are required to be in the `PATH`.

## Locations and Environment Variables

The following environmental variables are relevant for building the project.

| Environment Variable  | Purpose |
| --------------------  | ------------- |
| `QMAKE_SPEC`              | The [mkspec](https://forum.qt.io/topic/70970/what-is-mkspecs-used-for-how-to-configure-for-my-hardware) to compile to. Either `linux-g++` or `linux-clang`. Defaults to `linux-g++`.   |
| `USE_TIDY`              | If set a compilation database will be created and the project linted. Can only be used with `clang`.  |

If an environment variable isn't set a default value will be provided. The default values are shown in the table below.

## Building

With the programs above installed and environment variables set, go into the root folder of the repository and run `./build_scripts/linux/build_linux.sh`.

You'll have to copy the `res` directory into the resulting binary location, and add `third-party/openvr/lib/linux64/libopenvr_api.so` to your systems library path.

## Contributing

For full details, see [CONTRIBUTING.md](CONTRIBUTING.md).
