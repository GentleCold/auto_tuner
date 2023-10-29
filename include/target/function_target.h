//
// Created by GentleCold on 2023/10/21.
//

#ifndef AUTO_TUNER_FUNCTION_TARGET_H
#define AUTO_TUNER_FUNCTION_TARGET_H

#include <cassert>

#include "target.h"

namespace tune {

template <typename T>
struct func_traits;

template <typename R, typename... Args>
struct func_traits<R(Args...)> {
  using return_type = R;
  using arg_types = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct func_traits<R (*)(Args...)> : func_traits<R(Args...)> {};

template <typename... Args>
constexpr auto apply(auto&& f, std::vector<std::variant<Args...>>& args) {
  using func_args = typename func_traits<std::decay_t<decltype(f)>>::arg_types;
  constexpr auto func_args_num = std::tuple_size_v<func_args>;
  if (args.size() != func_args_num) {
    throw std::runtime_error("Wrong number of arguments");
  }

  [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    f(std::get<std::tuple_element_t<Is, func_args>>(args[Is])...);
  }
  (std::make_index_sequence<func_args_num>{});
}

template <typename F>
class FunctionTarget : public Target {
 public:
  FunctionTarget() = default;

  explicit FunctionTarget(const F& func) { _f = func; }

  void run(const ParamChoice& p, Optimization options) override;

 private:
  F _f;
};

template <typename F>
void FunctionTarget<F>::run(const ParamChoice& p, Optimization options) {
  // convert ParamChoice to a vector
  size_t num_of_args = p.size();
  std::vector<ParamType> args(num_of_args);

  for (const auto& arg : p) {
    const int index = std::stoi(arg.first);
    assert(index >= 0 && index < num_of_args);
    args[index] = arg.second.value();
  }

  // run the func
  std::cout << "Run func with: " << p << '\n';

  size_t total_time = 0;
  bool if_timeout = false;

  // todo: multiple thread
  for (auto i = 0; i < _num_of_runs; ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    apply(_f, args);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    total_time += duration.count();

    if (_timeout && total_time > _timeout) {
      std::cout << "Timeout...\n";
      total_time = _timeout;
      if_timeout = true;
      break;
    }
  }

  double run_time = (double)total_time / (double)(1000 * _num_of_runs);

  // Store result
  RunResult r(p, "", run_time, if_timeout);
  _rs.push_back(r);
}

}  // namespace tune

#endif  // AUTO_TUNER_FUNCTION_TARGET_H
