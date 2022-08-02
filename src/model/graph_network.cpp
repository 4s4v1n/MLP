#include "network.h"

namespace s21 {

void Neuron::SetWeights(const std::vector<double> &w) { m_weights = w; }

double &Neuron::GetWeight(size_t index) { return m_weights[index]; }

void Neuron::SetValue(double value) { m_value = value; }

double Neuron::GetValue() { return m_value; }

std::vector<double> GraphNetwork::GenerateWeights(size_t nOfWeights) {
  std::vector<double> weights(nOfWeights);
  for (size_t i = 0; i < nOfWeights; i++) {
    weights[i] = ((int)gen() % 10000) * 0.0001;
  }
  return weights;
}

auto GraphNetwork::SetLayers(std::vector<LayersInfo> info) -> void {
  m_topology.clear();
  for (int i = 0; i < m_neurons.size(); i++) {
    m_neurons[i].clear();
  }
  m_neurons.clear();
  m_biases.clear();
  for (size_t i = 0; i < info.size(); ++i) {
    m_topology.emplace_back(info[i]);
  }

  std::random_device rd;
  std::mt19937 temp(rd());
  m_generator = temp;

  for (int i = 0; i < m_topology.size(); i++) {
    m_neurons.push_back(Layer());
    m_biases.push_back(0);
    for (int j = 0; j < m_topology[i]; j++) {
      Neuron newNeuron;
      if (i != m_topology.size() - 1)
        newNeuron.SetWeights(GenerateWeights(m_topology[i + 1]));
      m_neurons[i].push_back(newNeuron);
    }
  }
}

auto GraphNetwork::SetLayers(std::initializer_list<size_t> items) -> void {
  m_topology.clear();
  for (int i = 0; i < m_neurons.size(); i++) {
    m_neurons[i].clear();
  }
  m_neurons.clear();
  m_biases.clear();

  std::vector<size_t> temp_v(items);
  m_topology = temp_v;

  std::random_device rd;
  std::mt19937 temp(rd());
  m_generator = temp;

  for (int i = 0; i < m_topology.size(); i++) {
    m_neurons.push_back(Layer());
    m_biases.push_back(0);
    for (int j = 0; j < m_topology[i]; j++) {
      Neuron newNeuron;
      if (i != m_topology.size() - 1)
        newNeuron.SetWeights(GenerateWeights(m_topology[i + 1]));
      m_neurons[i].push_back(newNeuron);
    }
  }
}

void GraphNetwork::FeedInitValues(const std::vector<double> &vals) {
  for (size_t i = 0; i < m_neurons[0].size(); i++) {
    m_neurons[0][i].SetValue(vals[i]);
  }
}
void GraphNetwork::FeedForward() {
  for (size_t layer = 0; layer < m_neurons.size() - 1; layer++) {
    for (size_t i = 0; i < m_neurons[layer + 1].size(); i++) {
      double sum = 0.;
      for (size_t j = 0; j < m_neurons[layer].size(); j++) {
        sum +=
            m_neurons[layer][j].GetWeight(i) * m_neurons[layer][j].GetValue();
      }
      m_neurons[layer + 1][i].SetValue(ActivateFunction(sum + m_biases[layer]));
    }
  }
}

auto GraphNetwork::ActivateFunction(double value) -> double {
  return 1.0 / (1.0 + exp(-value));
}

auto GraphNetwork::ActicateFunctionDerivative(double value) -> double {
  return value * (1 - value);
}

void GraphNetwork::GetLocalGrads(std::vector<double> &LocalGrads,
                                 const std::vector<double> &ExpectedValues,
                                 size_t layer) {
  if (layer == m_neurons.size() - 1) {
    // Получаем первые локальные градиенты из разницы полученного значения на
    // выходных нейронах и ожидаемого значения

    for (size_t i = 0; i < m_neurons[layer].size(); i++) {
      double error = ExpectedValues[i] - m_neurons[layer][i].GetValue();
      LocalGrads.push_back(
          error * ActicateFunctionDerivative(m_neurons[layer][i].GetValue()));
    }
  } else {
    // иначе получаем новые градиенты, умножая старые градиенты на веса
    // предыдущего слоя
    std::vector<double> NewGrads;

    for (size_t i = 0; i < m_neurons[layer].size(); i++) {
      double teta = 0.;

      for (size_t j = 0; j < m_neurons[layer + 1].size(); j++) {
        teta += LocalGrads[j] * m_neurons[layer][i].GetWeight(j);
      }
      NewGrads.push_back(
          teta * ActicateFunctionDerivative(m_neurons[layer][i].GetValue()));
    }
    LocalGrads = NewGrads;
  }
}

void GraphNetwork::BackPropagation(std::vector<double> &ExpectedValues) {
  std::vector<double> localGrads;
  // #pragma omp parallel for
  for (int i = m_neurons.size() - 2; i >= 0; i--) {
    GetLocalGrads(localGrads, ExpectedValues, i + 1);

    for (size_t j = 0; j < m_neurons[i].size(); j++) {
      for (size_t k = 0; k < m_neurons[i + 1].size(); k++) {
        m_neurons[i][j].GetWeight(k) +=
            m_step * localGrads[k] * m_neurons[i][j].GetValue();
      }
    }
  }
}

void GraphNetwork::SaveWeights(std::string filename) {
  std::fstream file;
  file.open(filename, file.out);
  file << "Network weights" << std::endl;
  for (int i = 0; i < m_topology.size(); i++) {
    file << m_topology[i] << ' ';
  }
  file << '\n';
  for (size_t layer = 0; layer < m_neurons.size() - 1; layer++) {
    for (size_t j = 0; j < m_neurons[layer + 1].size(); j++) {
      for (size_t i = 0; i < m_neurons[layer].size(); i++) {
        file << m_neurons[layer][i].GetWeight(j) << std::endl;
      }
    }
  }
  file.close();
}

auto GraphNetwork::CheckTopology(const std::vector<size_t> &topology) -> bool {
  bool res = true;
  if (topology.size() != m_topology.size()) {
    res = false;
  } else {
    for (int i = 0; i < topology.size(); i++) {
      if (m_topology[i] != topology[i]) res = false;
    }
  }
  return res;
}

auto GraphNetwork::GetWeights(std::string filename) -> bool {
  std::fstream file;
  file.open(filename, file.in);
  /// сделать обработку ошибок
  if (!file.is_open()) {
    return false;
  }

  std::string checkFile;
  std::getline(file, checkFile, '\n');
  if (checkFile != "Network weights") return false;

  std::string num;
  char c = 0;
  std::vector<size_t> topology;
  while (c != '\n' && !file.eof()) {
    c = 0;
    while (c != ' ' && c != '\n' && !file.eof()) {
      file.get(c);
      if (c != ' ' && c != '\n' && !file.eof()) {
        num += c;
      }
    }
    if (c != '\n') topology.push_back(std::stod(num));
    num = "";
  }
  if (CheckTopology(topology)) {
    for (size_t layer = 0; layer < m_neurons.size() - 1; layer++) {
      for (size_t j = 0; j < m_neurons[layer + 1].size(); j++) {
        for (size_t i = 0; i < m_neurons[layer].size(); i++) {
          std::getline(file, num, '\n');
          double number = std::stod(num);
          m_neurons[layer][i].GetWeight(j) = number;
        }
      }
    }
  } else {
    return false;
  }
  file.close();
  return true;
}

auto GraphNetwork::ShowResult() -> void {}

size_t GraphNetwork::GetResult() {
  size_t res = 0;
  double max = m_neurons.back()[0].GetValue();
  for (int i = 0; i < m_neurons.back().size(); i++) {
    if (max < m_neurons.back()[i].GetValue()) {
      max = m_neurons.back()[i].GetValue();
      res = i;
    }
  }
  return res;
}

auto GraphNetwork::GetResultVector() -> const std::vector<double> {
  std::vector<double> res(m_topology.back());
  for (int i = 0; i < res.size(); i++) {
    res[i] = m_neurons.back()[i].GetValue();
  }
  return res;
}
}  // namespace s21
