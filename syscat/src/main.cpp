#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "fmt/color.h"
#include "fmt/core.h"

#include "cli-params.h"
#include "utils.h"

#include <cstdio>
#include <dirent.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/stat.h>
#include <vector>

void print_file(const char *file_path);

int main(int argc, char **argv) {

  syscat::Options cli_opts;

  CLI::App app{"syscat: minimal cat clone"};
  app.add_option("path/to/file", cli_opts.file_path, "Path to file")
      ->required();

  CLI11_PARSE(app, argc, argv);

  {
    // what is current dir
    constexpr int BUFFER_SIZE = 256;
    char current_dir[BUFFER_SIZE];
    char *success = getcwd(current_dir, BUFFER_SIZE);

    if (not success) {
      syscat::print_error("Unknown error occured while getting current dir");
      return 1;
    }
    syscat::print_success(
        fmt::format("Current directory is '{}'", current_dir));
  }

  {
    const char *path = cli_opts.file_path.c_str();
    struct stat st;
    // TODO: wrap dir within dir-manager to use RAII and ensure safety
    stat(path, &st);
    if (S_ISREG(st.st_mode)) {
      if (int fd = open(path, O_RDONLY); fd != -1) {

        constexpr int BUFFER_SIZE = 256;
        char buffer[BUFFER_SIZE];
        int bytes_read;
        do {
          bytes_read = read(fd, buffer, BUFFER_SIZE);

          // write to stdout
          write(STDOUT_FILENO, buffer, bytes_read);
        } while (bytes_read >= 1);
      } else {
        syscat::print_error("Unknown error occured while reading the file");
      }
    } else {
      syscat::print_error(
          fmt::format("Path '{}' doesn't exist! Aborting...", path));
      return 2;
    }
  }

  return 0;
}
