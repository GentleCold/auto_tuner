//
// Created by GentleCold on 2023/10/19.
//

#ifndef AUTO_TUNER_DECISION_TREE_SEARCH_H
#define AUTO_TUNER_DECISION_TREE_SEARCH_H

#include "search/search.h"

namespace tune {

class DecisionTreeSearch : public Search {
 public:
  DecisionTreeSearch() = default;

  /***
   * Get best result by selecting random choices, meanwhile predict best choice
   * by decision tree regressor
   */
  RunResult get_best_result(Target &target, Optimization options) override;

  inline void set_ratio_of_samples(double ratio) { _ratio_of_samples = ratio; }

 private:
  /***
   * Index of all choices in param spacing
   */
  std::vector<std::vector<size_t>> _samples;

  double _ratio_of_samples = 0.5;

  /***
   * Generate all index of choices recursively, save to this._samples
   */
  void _generate_all_samples(std::vector<size_t> &sample, size_t key_index);

  /***
   * Construct choice by index
   * @param s Index of choice
   * @return A param choice
   */
  ParamChoice _sample2choice(const std::vector<size_t> &s);
};

}  // namespace tune

#endif  // AUTO_TUNER_DECISION_TREE_SEARCH_H
