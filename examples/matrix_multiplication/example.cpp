//
// Created by GentleCold on 2023/10/17.
//
#include <auto_tuner.h>

int main() {
  //! You can choose different target and search method according to template
  tune::AutoTuner<tune::CmdlineTarget, tune::DecisionTreeSearch> auto_tuner{};

  //! This is an example
  //  tune::AutoTuner<tune::CmdlineTarget, tune::GridSearch> auto_tuner{};

  //! Init CmdlineTarget with pre cmdline and run(target) cmdline
  auto_tuner.init_target(
      "gcc matrix_multiplication.c -o matrix_multiplication OPTIMIZATION",
      "./matrix_multiplication BUFFER_SIZE");

  //! Set possible params
  auto_tuner.set_params("OPTIMIZATION", "-O0", "-O1", "-O2", "-O3");
  auto_tuner.set_params("BUFFER_SIZE", 8, 16, 32, 64, 128);

  //! Set some configs
  //  auto_tuner.set_number_of_runs(1);
  //  auto_tuner.set_timeout(1000);
  auto_tuner.set_ratio_of_samples(0.4);

  //! Get best result by search method, and you can specify the optimization
  //! goal, if choose OUTPUT, the timeout will not work
  //  auto_tuner.get_best_result(tune::Optimization::MAX_OUTPUT_DOUBLE);
  auto_tuner.get_best_result(tune::Optimization::MIN_OUTPUT_DOUBLE);
  //  auto_tuner.get_best_result(tune::Optimization::MIN_RUNTIME);

  //! You can print all results for further analyse
  auto_tuner.print_all_results();

  //! Or you can save them to file
  auto_tuner.print_all_results("result.txt");

  return 0;
}