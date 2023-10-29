//
// Created by GentleCold on 2023/10/18.
//

#include <gtest/gtest.h>

#include "target/cmdline_target.h"

TEST(Target, run) { /* NOLINT */
  tune::CmdlineTarget t{};
  t.set_pre_cmd("ls");
  t.set_run_cmd("ls -l");
  tune::ParamChoice p;
  t.run(p);
  EXPECT_EQ(t.size_of_results(), 1);
}
