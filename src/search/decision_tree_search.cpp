//
// Created by GentleCold on 2023/10/19.
//
#include "search/decision_tree_search.h"

#include <mlpack/core.hpp>
#include <mlpack/methods/decision_tree/decision_tree_regressor.hpp>

//#include <mlpack.hpp>

namespace tune {

arma::mat _vec2mat(const std::vector<std::vector<size_t>>& v) {
  arma::mat r(v.size(), v[0].size());
  for (size_t i = 0; i < v.size(); ++i) {
    for (size_t j = 0; j < v[i].size(); ++j) {
      r(i, j) = static_cast<double>(v[i][j]);
    }
  }
  return r;
}

RunResult DecisionTreeSearch::get_best_result(Target& target,
                                              Optimization options) {
  std::cout << "Starting DecisionTreeSearch...\n";
  target.clear_results();
  // revert param to index
  _samples.clear();
  std::vector<size_t> sample;
  _generate_all_samples(sample, 0);

  // chose sample randomly
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(_samples.begin(), _samples.end(), gen);

  int train_size =
      _samples.size() * _ratio_of_samples; /* NOLINT size_t -> double */

  std::vector<std::vector<size_t>> train_samples(_samples.begin(),
                                                 _samples.begin() + train_size);
  for (const auto& s : train_samples) {
    ParamChoice p = _sample2choice(s);
    target.run(p, options);
  }

  // handle results
  auto results = target.get_all_results();
  arma::mat train_data = _vec2mat(train_samples);
  arma::mat all_data = _vec2mat(_samples);

  // the model need P*N dataset
  train_data = arma::trans(train_data);
  all_data = arma::trans(all_data);

  // mlpack::BayesianLinearRegression model;
  mlpack::DecisionTreeRegressor model;

  // get labels
  std::vector<double> doubles;
  for (const auto& tuple : results) {
    if (options == Optimization::MIN_RUNTIME) {
      doubles.push_back(std::get<2>(tuple));
    } else if (options == Optimization::MIN_OUTPUT_DOUBLE ||
               options == Optimization::MAX_OUTPUT_DOUBLE) {
      doubles.push_back(std::stod(std::get<1>(tuple)));
    }
  }
  arma::Row<double> label(doubles.data(), doubles.size());

  // train decision tree model and predict
  model.Train(train_data, label, 1);
  arma::Row<double> predict;
  model.Predict(all_data, predict);

  // run predicted best results
  if (options == Optimization::MIN_RUNTIME ||
      options == Optimization::MIN_OUTPUT_DOUBLE) {
    auto min_value = predict.min();
    for (size_t i = train_size; i < predict.n_elem; ++i) {
      if (predict[i] == min_value) {
        ParamChoice p = _sample2choice(_samples[i]);
        std::cout << "Predicted choice: " << p << '\n';
        target.run(_sample2choice(_samples[i]), options);
      }
    }
  } else {
    auto max_value = predict.max();
    for (size_t i = train_size; i < predict.n_elem; ++i) {
      if (predict[i] == max_value) {
        ParamChoice p = _sample2choice(_samples[i]);
        std::cout << "Predicted choice: " << p << '\n';
        target.run(_sample2choice(_samples[i]), options);
      }
    }
  }

  return target.get_best_result(options);
}

void DecisionTreeSearch::_generate_all_samples(/* NOLINT recursive */
                                               std::vector<size_t>& sample,
                                               size_t key_index) {
  if (key_index == _param_keys.size()) {
    _samples.push_back(sample);
    return;
  }

  std::string key = _param_keys[key_index];
  size_t value_size = _params_spacing.at(key).size();
  for (auto i = 0; i < value_size; ++i) {
    sample.push_back(i);
    _generate_all_samples(sample, key_index + 1);
    sample.pop_back();
  }
}

ParamChoice DecisionTreeSearch::_sample2choice(const std::vector<size_t>& s) {
  ParamChoice p;
  auto key_size = s.size();
  // construct choice
  for (size_t j = 0; j < key_size; ++j) {
    const auto& key = _param_keys[j];
    p[key] = _params_spacing[key][s[j]];
  }
  return p;
}

}  // namespace tune