/**
 * @file network.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#ifndef S21_MLP_NETWORK_H
#define S21_MLP_NETWORK_H

#include <cmath>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "../matrix/s21_matrix_oop.h"

namespace s21 {

enum LayersInfo {

  /**
   * @enum LayersInfo
   * @brief Enumeration of possible positions of layers of neurons
   */

  INPUT = 784,
  HIDDEN = 140,
  OUTPUT = 26
};

enum class NetworkType {

  /**
   * @enum NetworkType
   * @brief Enumeration of possible types of network
   */

  MATRIX = 0,
  GRAPH = 1
};

class Neuron {
 public:
  void SetWeights(const std::vector<double> &w);
  double &GetWeight(size_t index);
  void SetValue(double);
  double GetValue();

 private:
  std::vector<double> m_weights;
  void ActivateFunction();
  double m_value{};
};

struct Metrics {
  double accuracy;
  double precision;
  double recall;
  double f_measure;
  double ed_time;

  auto Calculate(int tp, int fp, int tn, int fn, int dataSize) -> void;
};

typedef std::vector<Neuron> Layer;

class NetworkInterface {
  /**
   * @interface NetworkInterface
   * @brief Abstract class for interaction with two types of neural networks
   */

 public:
  NetworkInterface() = default;
  virtual ~NetworkInterface() = default;

 public:
  virtual auto BackPropagation(std::vector<double> &ExpectedValues) -> void = 0;
  virtual auto FeedForward() -> void = 0;
  virtual auto FeedInitValues(const std::vector<double> &values) -> void = 0;
  virtual auto ShowResult() -> void = 0;
  virtual auto SaveWeights(std::string filename) -> void = 0;
  virtual auto GetWeights(std::string filename) -> bool = 0;
  virtual auto SetLayers(std::vector<LayersInfo> info) -> void = 0;
  virtual auto SetLayers(std::initializer_list<size_t> items) -> void = 0;

  virtual auto GetResult() -> size_t = 0;
  virtual auto GetResultVector() -> const std::vector<double> = 0;

 public:
  auto TestNet(const std::string &fileName, const double DataCoef) -> Metrics;
  auto Train(const std::string &fileName, const size_t epochs)
      -> std::vector<double>;
  auto CrossValidation(const std::string &fileName, const size_t k)
      -> std::vector<double>;

  auto GetLayersVector(int number) const -> std::vector<LayersInfo>;
  auto GetTopology() const -> const std::vector<size_t> &;

 protected:
  std::vector<size_t> m_topology;
  std::mt19937 m_generator;
};

class MatrixNetwork : public NetworkInterface {
  /**
   *  @class MatrixNetwork
   *  @brief Implementation of a neural network in a matrix representation
   */

 public:
  auto FeedForward() -> void override;
  auto FeedInitValues(const std::vector<double> &vals) -> void override;
  auto BackPropagation(std::vector<double> &ExpectedValues) -> void override;
  auto ShowResult() -> void override;
  auto SaveWeights(std::string filename) -> void override;
  auto GetWeights(std::string filename) -> bool override;

  auto GetResult() -> size_t override;
  auto GetResultVector() -> const std::vector<double> override;

  auto SetLayers(std::vector<LayersInfo> info) -> void override;
  auto SetLayers(std::initializer_list<size_t> items) -> void override;

  MatrixNetwork() = default;
  ~MatrixNetwork() = default;

 private:
  auto InitWeightMatrix(S21Matrix &matrix) -> void;
  auto randomWeight() -> double;
  auto ActivateFunction(double value) -> double;
  auto GetLocalGrads(std::vector<double> &LocalGrads,
                     const std::vector<double> &ExpectedValues, size_t layer)
      -> void;
  auto CheckTopology(const std::vector<size_t> &topology) -> bool;
  auto ActicateFunctionDerivative(double value) -> double;

  std::vector<S21Matrix> m_neurons;
  std::vector<S21Matrix> m_weights;
  std::vector<double> m_biases;

  static constexpr double step = 0.08;
};

class GraphNetwork : public NetworkInterface {
  /**
   *  @class GraphNetwork
   *  @brief Implementation of a neural network in a graph representation
   */

 public:
  auto FeedForward() -> void override;
  auto FeedInitValues(const std::vector<double> &vals) -> void override;
  auto BackPropagation(std::vector<double> &ExpectedValues) -> void override;
  auto ShowResult() -> void override;
  auto SaveWeights(std::string filename) -> void override;
  auto GetWeights(std::string filename) -> bool override;

  auto GetResult() -> size_t override;
  auto GetResultVector() -> const std::vector<double> override;

  GraphNetwork() = default;
  ~GraphNetwork() = default;

  auto SetLayers(std::vector<LayersInfo> info) -> void override;
  auto SetLayers(std::initializer_list<size_t> items) -> void override;

 private:
  std::mt19937 gen;
  std::vector<double> GenerateWeights(size_t nOfWeights);

  void GetLocalGrads(std::vector<double> &LocalGrads,
                     const std::vector<double> &ExpectedValues, size_t layer);
  auto ActivateFunction(double value) -> double;
  auto ActicateFunctionDerivative(double value) -> double;

  auto CheckTopology(const std::vector<size_t> &topology) -> bool;

  std::vector<Layer> m_neurons;
  std::vector<double> m_biases;
  static constexpr double m_step = 0.08;
};

}  // namespace s21
#endif
