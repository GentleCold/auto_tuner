//
// Created by GentleCold on 2023/10/18.
//

#ifndef AUTO_TUNER_COMMON_H
#define AUTO_TUNER_COMMON_H

#include <iostream>
#include <unordered_map>
#include <variant>

namespace tune {

/***
 * Possible value type, int/double/char/std::string, with to_string method
 */
class ParamValue {
 public:
  ParamValue() = default;
  explicit ParamValue(std::variant<int, double, char, std::string>&& v)
      : _p(std::forward<decltype(v)>(v)) {}

  [[nodiscard]] std::string to_string() const {
    std::string str = std::visit(
        [](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, std::string>)
            return arg;
          else
            return std::to_string(arg);
        },
        _p);
    return str;
  }

  [[nodiscard]] std::variant<int, double, char, std::string> value() const {
    return _p;
  }

 private:
  // todo: convert variant to any
  std::variant<int, double, char, std::string> _p;
};

/***
 * Supported parameter types
 */
using ParamType = std::variant<int, double, char, std::string>;

/***
 * A choice of param, key with a certain value
 */
using ParamChoice = std::unordered_map<std::string, ParamValue>;

/***
 * Run results, they are ParamChoice, output of program, runtime, timeout respectively
 */
using RunResult = std::tuple<ParamChoice, std::string, double, bool>;

std::ostream& operator<<(std::ostream& os, const ParamChoice& p);

std::ostream& operator<<(std::ostream& os, const RunResult& r);

/***
 * Possible optimization goal
 * MIN_RUNTIME - minimize run time test by AutoTuner
 * MIN_OUTPUT_DOUBLE - minimize output(convert to double) of program
 * MAX_OUTPUT_DOUBLE - maximize output(convert to double) of program
 */
enum class Optimization { MIN_RUNTIME, MIN_OUTPUT_DOUBLE, MAX_OUTPUT_DOUBLE };

}  // namespace tune

#endif  // AUTO_TUNER_COMMON_H
