//
// Created by GentleCold on 2023/10/19.
//
#include "common.h"
namespace tune {
std::ostream& operator<<(std::ostream& os, const ParamChoice& p) {
  os << "{ ";
  for (const auto& pair : p) {
    os << pair.first << ": " << pair.second.to_string() << ", ";
  }
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const RunResult& r) {
  os << "Param Choice: " << std::get<0>(r) << '\n';
  os << std::get<1>(r);
  os << "Run Time: " << std::get<2>(r) << "ms\n";
  os << "If timeout: " << std::get<3>(r) << '\n';

  return os;
}

}  // namespace tune