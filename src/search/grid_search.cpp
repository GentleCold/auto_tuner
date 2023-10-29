//
// Created by GentleCold on 2023/10/18.
//

#include "search/grid_search.h"

namespace tune {

RunResult GridSearch::get_best_result(Target& target, Optimization options) {
  std::cout << "Starting GridSearch...\n";
  target.clear_results();
  ParamChoice p;
  _run_all_param_choices(target, p, 0, options);
  return target.get_best_result(options);
}

void GridSearch::_run_all_param_choices(Target& target, /* NOLINT */
                                        ParamChoice& p, size_t key_index,
                                        Optimization options) {
  if (key_index == _param_keys.size()) {
    target.run(p, options);
    return;
  }

  std::string key = _param_keys[key_index];
  const auto& values = _params_spacing.at(key);
  for (const auto& value : values) {
    p[key] = value;
    _run_all_param_choices(target, p, key_index + 1, options);
  }
}

}  // namespace tune
