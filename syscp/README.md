# syscp

`cp` like utility to copy files, part of [sys-callers](https://github.com/rathod-sahaab/sys-callers) apps.

Project setup using template [rathod-sahaab/cpp-conan-template](https://github.com/rathod-sahaab/cpp-conan-template).

## Usage

```sh
syscp path/to/source path/to/dest
```

You can add to `$PATH` or install but don't (not yet atleast).

```
syscp: minimal cat clone
Usage: bin/syscp [OPTIONS] source dest

Positionals:
  source TEXT REQUIRED        Path to source file
  dest TEXT REQUIRED          Path to destination file

Options:
  -h,--help                   Print this help message and exit
```

examples:

```sh
syscp source.txt destination.txt
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

Binary/app can be found in `build/bin/` which will be same as the project name `syscp`.
