#ifndef CLI_PARAMS_H
#define CLI_PARAMS_H

#include <string>
#include <vector>

namespace syscat {
struct Options {
  std::vector<std::string> file_path;
};
} // namespace syscat

#endif // CLI_PARAMS_H
