//
// Created by GentleCold on 2023/10/21.
//

#include <auto_tuner.h>

#include <random>

void test_func(int arg0, double arg1, std::string arg2) {
  //  std::cout << arg2 << '\n';
  auto j = arg0 * arg1;
  auto sum = 0.0;
  std::random_device rd;
  std::mt19937 gen(rd());
  for (auto i = 0; i < j; ++i) {
    // loop...
    std::uniform_int_distribution<> dis(0, 1);
    int random_number = dis(gen);
    sum += random_number;
  }
}

int main() {
  auto func_obj = test_func;
  tune::AutoTuner<tune::FunctionTarget<decltype(func_obj)>,
                  tune::DecisionTreeSearch>
      auto_tuner{};
  auto_tuner.init_target(func_obj);

  //! Note that the type needs to be strictly corresponding
  auto_tuner.set_params("0", 10000000, 20000000, 30000000, 40000000);
  auto_tuner.set_params("1", (double)1, (double)2, (double)3, (double)4,
                        (double)5);
  auto_tuner.set_params("2", "str1", "str2");

  auto_tuner.set_number_of_runs(5);
  auto_tuner.set_timeout(3000);
  auto_tuner.set_ratio_of_samples(0.5);

  //! OUTPUT goal will not work in Func
  // auto_tuner.get_best_result(tune::Optimization::MIN_OUTPUT_DOUBLE);
  auto_tuner.get_best_result(tune::Optimization::MIN_RUNTIME);

  auto_tuner.print_all_results();

  return 0;
}