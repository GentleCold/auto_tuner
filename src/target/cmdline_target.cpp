//
// Created by GentleCold on 2023/10/18.
//
#include "target/cmdline_target.h"

#include <chrono>
#include <iostream>
#include <memory>

namespace tune {

void CmdlineTarget::run(const ParamChoice& p, Optimization options) {
  if (_run_cmd.empty()) {
    std::cout << "No run cmd set!"
              << "\n";
    return;
  }
  std::string new_pre_cmd = _pre_cmd;
  std::string new_run_cmd = _run_cmd;

  // Substitute cmdline
  for (auto& param : p) {
    size_t pos;
    std::string sub = param.second.to_string();
    if ((pos = new_pre_cmd.find(param.first)) != std::string::npos) {
      new_pre_cmd.replace(pos, param.first.length(), sub);
    } else if ((pos = new_run_cmd.find(param.first)) != std::string::npos) {
      new_run_cmd.replace(pos, param.first.length(), sub);
    } else {
      throw std::runtime_error("Cannot find key");
    }
  }

  // Run
  std::array<char, 128> buffer{};
  std::string output;
  double numeric_output = 0;

  if (!new_pre_cmd.empty()) {
    std::cout << "Run pre cmd: " << new_pre_cmd << "\n";
    if (std::system(new_pre_cmd.c_str()) != 0)
      throw std::runtime_error("Run pre cmd error");
  }

  std::cout << "Run run cmd: " << new_run_cmd << "\n";

  size_t total_time = 0;
  bool if_timeout = false;
  // todo: multiple thread
  for (auto i = 0; i < _num_of_runs; ++i) {
    if (options == Optimization::MAX_OUTPUT_DOUBLE ||
        options == Optimization::MIN_OUTPUT_DOUBLE) {
      if (output.empty()) output = "0";
      numeric_output += std::stod(output);
      output = "";
    } else {
      output += "Output" + std::to_string(i) + ": ";
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    FILE* pipe = popen(new_run_cmd.c_str(), "r");
    if (!pipe) {
      throw std::runtime_error("Run run cmd failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
      output += buffer.data();
    }

    if (WEXITSTATUS(pclose(pipe)) != 0) {
      throw std::runtime_error("Run run cmd error");
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    total_time += duration.count();

    if (options == Optimization::MIN_RUNTIME) {
      if (_timeout && total_time > _timeout) {
        std::cout << "Timeout...\n";
        total_time = _timeout;
        if_timeout = true;
        break;
      }
    }
  }

  double run_time = (double)total_time / (double)(1000 * _num_of_runs);

  if (options == Optimization::MAX_OUTPUT_DOUBLE ||
      options == Optimization::MIN_OUTPUT_DOUBLE) {
    numeric_output += std::stod(output);
    output = std::to_string(numeric_output / (double)_num_of_runs);
    output += '\n';
  }

  // Store result
  RunResult r(p, output, run_time, if_timeout);
  _rs.push_back(r);
}

}  // namespace tune
