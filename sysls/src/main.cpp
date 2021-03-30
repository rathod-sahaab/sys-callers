#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "fmt/color.h"
#include "fmt/core.h"

#include "cli-params.h"

#include <cstdio>
#include <dirent.h>
#include <unistd.h>

#include <sys/dir.h>

int main(int argc, char **argv) {

  sysls::Options cli_opts;

  CLI::App app{"sysls: minimal ls clone"};
  app.add_flag("-c, --color", cli_opts.color, "enable color");

  CLI11_PARSE(app, argc, argv);

  {
    // what is current dir
    constexpr int BUFFER_SIZE = 256;
    char current_dir[BUFFER_SIZE];
    char *success = getcwd(current_dir, BUFFER_SIZE);

    if (not success) {
      perror("getcwd");
      fmt::print(fmt::fg(fmt::color::red),
                 "ERROR: encountered an error while getting current dir\n");
      return 1;
    }
    fmt::print(fmt::fg(fmt::color::green), "SUCCESS: ");
    fmt::print("Current directory is '{}'\n", current_dir);
  }

  return 0;
}
