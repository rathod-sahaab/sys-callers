#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "fmt/color.h"
#include "fmt/core.h"

#include "cli-params.h"
#include "fmt/format.h"
#include "utils.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/stat.h>
#include <vector>

void print_file(const char *file_path);

bool all_paths_are_valid(const std::vector<std::string> &filenames) {
  return std::all_of(
      filenames.cbegin(), filenames.cend(), [](const std::string &file) {
        struct stat st;
        stat(file.c_str(), &st);
        if (not S_ISREG(st.st_mode)) {
          syscat::print_error(fmt::format("Path '{}' cannot be read!", file));
          return false;
        }
        return true;
      });
}

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
    if (not all_paths_are_valid(cli_opts.file_path)) {
      exit(1);
    }
    for (const auto &path : cli_opts.file_path) {

      if (int fd = open(path.c_str(), O_RDONLY); fd != -1) {

        constexpr int BUFFER_SIZE = 256;
        char buffer[BUFFER_SIZE];
        int bytes_read;
        do {
          bytes_read = read(fd, buffer, BUFFER_SIZE);

          // write to stdout
          write(STDOUT_FILENO, buffer, bytes_read);
        } while (bytes_read >= 1);
      } else {
        syscat::print_error(fmt::format(
            "Unknown error occured while reading the file '{}'\n", path));
      }
    }
  }

  return 0;
}
