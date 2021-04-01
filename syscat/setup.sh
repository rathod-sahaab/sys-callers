mkdir build && cd build
conan install ..
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 # generates compile_commands.json
