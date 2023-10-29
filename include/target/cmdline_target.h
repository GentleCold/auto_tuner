//
// Created by GentleCold on 2023/10/18.
//

#ifndef AUTO_TUNER_CMDLINE_TARGET_H
#define AUTO_TUNER_CMDLINE_TARGET_H

#include "target.h"

#include <utility>

namespace tune {

class CmdlineTarget : public Target {
 public:
  CmdlineTarget() = default;

  /***
   * Construct func
   */
  CmdlineTarget(std::string pre, std::string run)
      : _pre_cmd(std::move(pre)), _run_cmd(std::move(run)) {}

  /***
   * Copy construction func
   */
  CmdlineTarget(const CmdlineTarget &t) : Target(t) {
    _pre_cmd = t._pre_cmd;
    _run_cmd = t._run_cmd;
  }

  /***
   * Run the pre cmdline (gcc for example) and run cmdline (run the program) of
   * the certain params choice and store the result
   * @param p A param choice
   */
  void run(const ParamChoice &p, Optimization options) override;

  inline void set_pre_cmd(const std::string &cmdline) { _pre_cmd = cmdline; }

  inline void set_run_cmd(const std::string &cmdline) { _run_cmd = cmdline; }

 private:
  std::string _pre_cmd;
  std::string _run_cmd;
};

}  // namespace tune

#endif  // AUTO_TUNER_CMDLINE_TARGET_H