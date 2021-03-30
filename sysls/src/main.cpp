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
#include <vector>

void list_dir(DIR *dir);

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
    sysls::print_success(fmt::format("Current directory is '{}'", current_dir));
  }

  {
    const char *path = cli_opts.dir.c_str();
    if (access(path, F_OK) != -1) {
      // TODO: wrap dir within dir-manager to use RAII and ensure safety
      if (DIR *dir = opendir(path); dir != NULL) {
        sysls::print_success(fmt::format("Path '{}' is a directory", path));
        list_dir(dir);
        closedir(dir);
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

void list_dir(DIR *dir) {
  // WARNING: doesn't own dir don't close!

  struct dirent *dir_entry;

  std::vector<std::string> directories, files;

  while ((dir_entry = readdir(dir)) != NULL) {
    if (dir_entry->d_type == DT_DIR) {
      directories.emplace_back(dir_entry->d_name);
    } else {

      files.emplace_back(dir_entry->d_name);
    }
  }

  for (const auto &directory : directories) {
    if (directory.front() != '.') {
      // condition filters out current . , parent .. , and hidden files
      // TODO: add_option to not filter out current and parent;
      // TODO: add option to show hidden files
      fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "{}/\n",
                 directory);
    }
  }

  for (const auto &file : files) {
    fmt::print("{}\n", file);
  }
}
