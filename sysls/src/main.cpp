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

#include <sys/dir.h>

int main(int argc, char **argv) {

  sysls::Options cli_opts;

  CLI::App app{"sysls: minimal ls clone"};
  app.add_flag("-c, --color", cli_opts.color, "enable color")
      ->default_val(false);
  app.add_option("path/to/file", cli_opts.dir,
                 "Path to directory to list contents of")
      ->default_val(".");

  CLI11_PARSE(app, argc, argv);

  {
    // what is current dir
    constexpr int BUFFER_SIZE = 256;
    char current_dir[BUFFER_SIZE];
    char *success = getcwd(current_dir, BUFFER_SIZE);

    if (not success) {
      sysls::print_error("Unknown error occured while getting current dir");
      return 1;
    }
    sysls::print_success(
        fmt::format("Current directory is '{}'\n", current_dir));
  }

  {
    const char *path = cli_opts.dir.c_str();
    if (access(path, F_OK) != -1) {
      if (DIR *dir = opendir(path); dir != NULL) {
        sysls::print_success(fmt::format("'{}' is a directory", path));
      } else {
        sysls::print_error(
            fmt::format("Path '{}' doesn't point to a directory", path));
      }
    } else {
      sysls::print_error(
          fmt::format("Path '{}' doesn't exist! Aborting...", path));
      return 2;
    }
  }

  return 0;
}
