//
// Created by GentleCold on 2023/10/17.
//

#ifndef AUTO_TUNER_AUTO_TUNER_H
#define AUTO_TUNER_AUTO_TUNER_H

#include <cassert>
#include <chrono>
#include <fstream>
#include <memory>
#include <type_traits>

#include "search/decision_tree_search.h"
#include "search/grid_search.h"
#include "target/cmdline_target.h"
#include "target/function_target.h"

namespace tune {

/***
 * Further packaging of Target and Search, select the corresponding interface
 * through the template
 * @tparam T Target(cmd / function)
 * @tparam S Search method
 */
template <typename T, typename S,
          typename = std::enable_if_t<std::is_base_of_v<Target, T> &&
                                      std::is_base_of_v<Search, S>>>
class AutoTuner {
 public:
  AutoTuner() = default;

  /***
   * Just call set_params func of Search class
   */
  template <typename... Args>
  inline void set_params(const std::string &key, Args &&...args) {
    search.set_params(key, std::forward<Args>(args)...);
  }

  /***
   * Init target according to different interfaces
   * @tparam Args Possible args to init target
   */
  template <typename... Args>
  inline void init_target(Args &&...args) {
    if constexpr (std::is_same_v<T, CmdlineTarget>) {
      static_assert(sizeof...(args) == 2);
    }
    target = T(args...);
  }

  /***
   * Run search method and print best result with total search time
   * @param option Optimization target
   */
  inline void get_best_result(Optimization option) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = search.get_best_result(target, option);
    std::cout << "----------BEST RESULT----------\n";
    std::cout << result;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    double run_time = (double)duration.count() / (1000);
    std::cout << "Total search time: " << run_time << "ms\n";
  }

  inline void print_all_results() {
    auto results = target.get_all_results();
    std::sort(results.begin(), results.end(),
              [](const auto &result1, const auto &result2) {
                return std::get<2>(result1) < std::get<2>(result2);
              });
    for (auto &result : results) {
      std::cout << "------------RESULT------------\n" << result;
    }
  }

  inline void print_all_results(const std::string &file_loc) {
    std::ofstream file(file_loc);
    if (!file.is_open()) {
      std::cout << "Open " << file_loc << " failed!";
      return;
    }
    auto results = target.get_all_results();
    std::sort(results.begin(), results.end(),
              [](const auto &result1, const auto &result2) {
                return std::get<2>(result1) < std::get<2>(result2);
              });
    for (auto &result : results) {
      file << "------------RESULT------------\n" << result;
    }
    file.close();
  }

  /***
   * Limited time of total run time, will not work if number of runs is 1
   * @param ms Million seconds
   */
  inline void set_timeout(size_t ms) { target.set_timeout(ms); }

  inline void set_number_of_runs(size_t num) { target.set_number_of_runs(num); }

  inline void set_ratio_of_samples(double ratio) {
    assert(ratio >= 0 && ratio <= 1);
    if constexpr (std::is_same_v<S, DecisionTreeSearch>) {
      search.set_ratio_of_samples(ratio);
    }
  };

  [[nodiscard]] inline size_t size_of_spacing() const {
    return search.size_of_spacing();
  }

  [[nodiscard]] inline size_t size_of_results() const {
    return target.size_of_results();
  }

 private:
  T target;
  S search;
};

}  // namespace tune

#endif  // AUTO_TUNER_AUTO_TUNER_H
