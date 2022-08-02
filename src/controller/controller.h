/**
 * @file controller.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>

#include "../model/network.h"

namespace s21 {

class Controller {
  /**
   * @class Controller
   * @brief Controller for different networks and settings
   */

 public:
  Controller() = default;
  ~Controller();

 public:
  auto SetNet(s21::NetworkType type, int hiddenLayers) -> void;
  auto GetNet() const -> NetworkInterface*;

 public:
  auto FeedForward() -> void;
  auto FeedInitValues(const std::vector<double>& values) -> void;
  auto GetResult() -> size_t;
  auto SaveWeights(const std::string& fileName) -> void;
  auto LoadWeights(const std::string& fileName) -> void;
  auto GetMetrics(const std::string& fileName, const double datasetUsage)
      -> Metrics;
  auto Train(const std::string& fileName, const size_t epochs)
      -> std::vector<double>;
  auto CrossValidation(const std::string& fileName, const size_t k)
      -> std::vector<double>;

 private:
  NetworkInterface* m_net{};
  s21::NetworkType m_netType{};
  int m_hiddenLayers{};
};

}  // namespace s21

#endif  // CONTROLLER_H
