# sysls

`ls` like utility to list files and directories, part of [sys-callers](https://github.com/rathod-sahaab/sys-callers) apps.

Project setup using template [rathod-sahaab/cpp-conan-template](https://github.com/rathod-sahaab/cpp-conan-template).
## Usage

```sh
path/to/sysls path/to/dir
```

You can add to `$PATH` or install but don't (not yet atleast).

```
Usage: build/bin/sysls [OPTIONS] [path/to/file]

Positionals:
  path/to/file TEXT=. Path to directory to list contents of

Options:
  -h,--help Print this help message and exit
  -c,--color=0 enable color
```

examples:

```sh
sysls
```

```sh
sysls ..
```

```sh
sysls ~ # home directory
```

## Setup

Run

```sh
./setup.sh
```

or

```sh
mkdir build && cd build
conan install ..
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 # generates compile_commands.json
```

## Compile

#### After file changes

```sh
cd build
make -jX
```

_**X**: number of CPU threads on your machine_

#### After dependencies changes

```sh
cd build
conan install ..
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```

## Binary

Binary/app can be found in `build/bin/` which will be same as the project name `sysls`.

