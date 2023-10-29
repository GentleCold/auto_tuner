//
// Created by GentleCold on 2023/10/17.
//

#ifndef AUTO_TUNER_SEARCH_H
#define AUTO_TUNER_SEARCH_H

#include <unordered_map>
#include <variant>
#include <vector>

#include "common.h"
#include "target/cmdline_target.h"

namespace tune {

class Search {
 public:
  Search() = default;
  /**
   * Add param with possible values
   * @tparam Args Can be int / double / char / std::string
   * @param key The name of the param, string
   * @param args The possible value of the param
   */
  template <typename... Args>
  void set_params(const std::string& key, Args&&... args) {
    std::vector<ParamValue> param_values;
    (param_values.emplace_back(std::forward<Args>(args)), ...);
    _params_spacing[key] = param_values;
    _param_keys.push_back(key);
  }

  /***
   * Needs to be implemented according to the search method
   * @param target Call run func of Target
   * @param options Optimization goal
   * @return run result
   */
  virtual RunResult get_best_result(Target& target, Optimization options) = 0;

  [[nodiscard]] inline size_t size_of_params() const {
    return _params_spacing.size();
  }

  [[nodiscard]] inline size_t size_of_spacing() const {
    size_t size = 1;
    for (const auto& p : _params_spacing) size *= p.second.size();
    return size;
  }

 protected:
  std::unordered_map<std::string, std::vector<ParamValue>> _params_spacing;
  std::vector<std::string> _param_keys;
};

}  // namespace tune

#endif  // AUTO_TUNER_SEARCH_H
