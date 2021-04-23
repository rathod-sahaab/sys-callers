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
#include <fcntl.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/stat.h>
#include <vector>

void print_file(const char *file_path);

int main(int argc, char **argv) {

  syscp::Options cli_opts;

  CLI::App app{"syscp: minimal cat clone"};
  app.add_option("source", cli_opts.source, "Path to source file")->required();
  app.add_option("dest", cli_opts.destination, "Path to destination file")
      ->required();

  CLI11_PARSE(app, argc, argv);

  {
    // what is current dir
    constexpr int BUFFER_SIZE = 256;
    char current_dir[BUFFER_SIZE];
    char *success = getcwd(current_dir, BUFFER_SIZE);

    if (not success) {
      syscp::print_error("Unknown error occured while getting current dir");
      return 1;
    }
    syscp::print_success(fmt::format("Current directory is '{}'", current_dir));
  }

  {

    struct stat st;
    stat(cli_opts.source.c_str(), &st);

    if (not S_ISREG(st.st_mode)) {
      exit(-1);
    }
    int source_fd = open(cli_opts.source.c_str(), O_RDONLY);
    int destination_fd = creat(cli_opts.destination.c_str(), S_IWUSR);
    if (source_fd != -1 and destination_fd != -1) {

      constexpr int BUFFER_SIZE = 256;
      char buffer[BUFFER_SIZE];
      int bytes_read;
      do {
        bytes_read = read(source_fd, buffer, BUFFER_SIZE);

        // write to stdout
        write(destination_fd, buffer, bytes_read);
      } while (bytes_read >= 1);

      syscp::print_success(fmt::format("Copied file '{}' to '{}'",
                                       cli_opts.source, cli_opts.destination));
    } else {
      syscp::print_error(fmt::format("Unknown error occured while reading the "
                                     "file '{}' or writing to file '{}'\n",
                                     cli_opts.source, cli_opts.destination));
    }
  }

  return 0;
}
