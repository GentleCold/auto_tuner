//
// Created by GentleCold on 2023/10/18.
//

#ifndef AUTO_TUNER_GRID_SEARCH_H
#define AUTO_TUNER_GRID_SEARCH_H

#include "search/search.h"

namespace tune {
class GridSearch : public Search {
 public:
  GridSearch() = default;

  /***
   * Get best result by traveling all choices
   */
  RunResult get_best_result(Target &target,
                              Optimization options) override;

 private:
  void _run_all_param_choices(Target &target, ParamChoice &p,
                               size_t key_index, Optimization options);
};

}  // namespace tune

#endif  // AUTO_TUNER_GRID_SEARCH_H
