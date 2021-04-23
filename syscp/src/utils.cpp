#include "utils.h"
#include "fmt/color.h"
#include "fmt/core.h"

namespace syscp {
void print_error(const std::string &msg) {
  const auto error_tag = fmt::format(fmt::fg(fmt::color::orange_red), "ERROR:");
  fmt::print(stderr, "{} {}\n", error_tag, msg);
}

void print_success(const std::string &msg) {
  const auto success_tag =
      fmt::format(fmt::fg(fmt::color::light_green), "SUCCESS:");
  fmt::print(stderr, "{} {}\n", success_tag, msg);
}

} // namespace syscp
