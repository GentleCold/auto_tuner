//
// Created by GentleCold on 2023/10/19.
//
#include "target/target.h"

namespace tune {

const RunResult& tune::Target::get_best_result(Optimization options) const {
  if (options == Optimization::MIN_RUNTIME) {
    return *std::min_element(
        _rs.begin(), _rs.end(), [](const auto& result1, const auto& result2) {
          return std::get<2>(result1) < std::get<2>(result2);
        });
  } else if (options == Optimization::MIN_OUTPUT_DOUBLE) {
    return *std::min_element(
        _rs.begin(), _rs.end(), [](const auto& result1, const auto& result2) {
          return std::stod(std::get<1>(result1)) < std::stod(std::get<1>(result2));
        });
  } else if (options == Optimization::MAX_OUTPUT_DOUBLE){
    return *std::max_element(
        _rs.begin(), _rs.end(), [](const auto& result1, const auto& result2) {
          return std::stod(std::get<1>(result1)) < std::stod(std::get<1>(result2));
        });
  } else {
    throw std::runtime_error("No optimization goal!");
  }
}

std::vector<RunResult> Target::get_all_results() {
  return _rs;
}

}  // namespace tune
