//
// Created by GentleCold on 2023/10/19.
//

#ifndef AUTO_TUNER_TARGET_H
#define AUTO_TUNER_TARGET_H

#include <string>
#include <vector>

#include "common.h"

namespace tune {

class Target {
 public:
  Target() = default;

  /***
   * Needs to be implemented according to different run way, store results to
   * this._rs
   * @param p A param choice
   */
  virtual void run(const ParamChoice &p, Optimization options) = 0;

  /***
   * Get best result from _rs according to options
   * @param options Optimization goal
   * @return best run result
   */
  [[nodiscard]] const RunResult &get_best_result(Optimization options) const;

  /***
   * Return all results
   * @return Sorted results
   */
  [[nodiscard]] std::vector<RunResult> get_all_results();

  [[nodiscard]] inline size_t size_of_results() const { return _rs.size(); }

  inline void set_timeout(size_t ms) { _timeout = 1000 * ms; }

  inline void set_number_of_runs(size_t num) {
    _num_of_runs = num;
  }

  inline void clear_results() {
    _rs.clear();
  }

 protected:
  std::vector<RunResult> _rs;
  /***
   * Microseconds
   */
  size_t _timeout = 0;
  size_t _num_of_runs = 1;
};

}  // namespace tune
#endif  // AUTO_TUNER_TARGET_H
