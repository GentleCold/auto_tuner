//
// Created by GentleCold on 2023/10/17.
//
#include <gtest/gtest.h>

#include <complex>

#include "search/grid_search.h"

TEST(Search, set_params) { /* NOLINT */
  tune::GridSearch p{};
  p.set_params("FLAG", "1", '1', 1, 1.11);
  EXPECT_EQ(p.size_of_params(), 1);
  p.set_params("ARG", "-c", "-d");
  EXPECT_EQ(p.size_of_params(), 2);
  EXPECT_EQ(p.size_of_spacing(), 8);
}

TEST(Search, set_params_performance) { /* NOLINT */
  tune::GridSearch p{};
  for (auto i = 0; i < 10; i++) {
    p.set_params("param" + std::to_string(i), 1, 2, '3', "4");
  }
  EXPECT_EQ(p.size_of_params(), 10);
  EXPECT_EQ(p.size_of_spacing(), std::pow(4, 10));

  for (auto i = 0; i < 10000; i++) {
    p.set_params("param" + std::to_string(i), 1);
  }
  EXPECT_EQ(p.size_of_params(), 10000);
  EXPECT_EQ(p.size_of_spacing(), 1);
}

TEST(Search, get_best_results) { /* NOLINT */
  tune::CmdlineTarget t{};
  tune::GridSearch g{};
  t.set_pre_cmd(
      "cd "
      "/mnt/d/EditorTXT/systemoptimization/10215501413/examples/"
      "matrix_multiplication/ && gcc matrix_multiplication.c -o "
      "matrix_multiplication OPTIMIZATION");
  t.set_run_cmd(
      "cd "
      "/mnt/d/EditorTXT/systemoptimization/10215501413/examples/"
      "matrix_multiplication/ && ./matrix_multiplication BUFFER_SIZE");
  g.set_params("OPTIMIZATION", "-O0", "-O1", "-O2", "-O3");
  g.set_params("BUFFER_SIZE", 8, 16, 32, 64, 128);
  g.get_best_result(t, tune::Optimization::MIN_RUNTIME);

  EXPECT_EQ(t.size_of_results(), 20);
}
