/**
 * @file matrix_network.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "network.h"

namespace s21 {

auto NetworkInterface::GetLayersVector(int number) const
    -> std::vector<LayersInfo> {
  std::vector<LayersInfo> resultVector;
  resultVector.emplace_back(LayersInfo::INPUT);
  for (int i = 0; i < number; ++i) {
    resultVector.emplace_back(LayersInfo::HIDDEN);
  }
  resultVector.emplace_back(LayersInfo::OUTPUT);
  return resultVector;
}

auto NetworkInterface::GetTopology() const -> const std::vector<size_t> & {
  return m_topology;
}

auto MatrixNetwork::SetLayers(std::vector<LayersInfo> info) -> void {
  m_topology.clear();
  for (int i = 0; i < m_neurons.size(); i++) {
    m_neurons[i].clear();
  }
  for (int i = 0; i < m_weights.size(); i++) {
    m_weights[i].clear();
  }
  m_weights.clear();
  m_neurons.clear();
  m_biases.clear();
  for (size_t i = 0; i < info.size(); ++i) {
    m_topology.emplace_back(info[i]);
  }
  std::random_device randDevice;
  std::mt19937 tempGenerator(randDevice());
  m_generator = tempGenerator;
  for (size_t i = 0; i < info.size(); ++i) {
    m_neurons.emplace_back(S21Matrix(info[i], 1));
    m_biases.emplace_back(0);
    if (i != info.size() - 1) {
      m_weights.emplace_back(S21Matrix(info[i + 1], info[i]));
      InitWeightMatrix(m_weights.back());
    }
  }
}

auto MatrixNetwork::SetLayers(std::initializer_list<size_t> items) -> void {
  std::vector<size_t> temp_v(items);
  m_topology = temp_v;

  for (int i = 0; i < m_neurons.size(); i++) {
    m_neurons[i].clear();
  }
  for (int i = 0; i < m_weights.size(); i++) {
    m_weights[i].clear();
  }
  m_weights.clear();
  m_neurons.clear();
  m_biases.clear();

  std::random_device randDevice;
  std::mt19937 tempGenerator(randDevice());
  m_generator = tempGenerator;
  for (size_t i = 0; i < m_topology.size(); ++i) {
    m_neurons.emplace_back(S21Matrix(m_topology[i], 1));
    m_biases.emplace_back(0);
    if (i != m_topology.size() - 1) {
      m_weights.emplace_back(S21Matrix(m_topology[i + 1], m_topology[i]));
      InitWeightMatrix(m_weights.back());
    }
  }
}

void MatrixNetwork::InitWeightMatrix(S21Matrix &matrix) {
  for (size_t i = 0; i < matrix.GetRow(); i++) {
    for (size_t j = 0; j < matrix.GetColumn(); j++) {
      matrix(i, j) = randomWeight();
    }
  }
}
auto MatrixNetwork::randomWeight() -> double {
  double rand = ((int)m_generator() % 10000) * 0.0001;
  return rand;
}

auto MatrixNetwork::ActivateFunction(double value) -> double {
  return 1.0 / (1.0 + exp(-value));
}

auto MatrixNetwork::ActicateFunctionDerivative(double value) -> double {
  return value * (1 - value);
}

auto MatrixNetwork::FeedInitValues(const std::vector<double> &values) -> void {
  for (size_t i = 0; i < values.size(); i++) {
    m_neurons[0][i][0] = values[i];
  }
}
/*--- Проход сигнала от первого слоя до последнего ---*/
auto MatrixNetwork::FeedForward() -> void {
  for (size_t i = 0; i < m_neurons.size() - 1; i++) {
    m_neurons[i + 1] = m_weights[i] * m_neurons[i];
    for (size_t j = 0; j < m_neurons[i + 1].GetRow(); j++) {
      m_neurons[i + 1](j, 0) =
          ActivateFunction(m_neurons[i + 1](j, 0) + m_biases[i]);
    }
  }
}

/**
 * @brief Получение локальных градиентов слоя
 * @param LocalGrads текущий вектор локальных градиентов
 * @param ExpectedValues Ожидаемые значения для выходных нейронов
 * @param layer индекс слоя матрицы весов, для которого вычисляются локальные
 * градиенты
 */
auto MatrixNetwork::GetLocalGrads(std::vector<double> &LocalGrads,
                                  const std::vector<double> &ExpectedValues,
                                  size_t layer) -> void {
  if (LocalGrads.empty()) {
    // Получаем первые локальные градиенты из разницы полученного значения на
    // выходных нейронах и ожидаемого значения

    for (size_t i = 0; i < m_neurons.back().GetRow(); i++) {
      double error = ExpectedValues[i] - m_neurons.back()[i][0];
      LocalGrads.push_back(error *
                           ActicateFunctionDerivative(m_neurons.back()[i][0]));
    }
  } else {
    // иначе получаем новые градиенты, умножая старые градиенты на веса
    // предыдущего слоя
    std::vector<double> NewGrads;

    for (size_t i = 0; i < m_weights[layer].GetColumn(); i++) {
      double teta = 0.;

      for (size_t j = 0; j < m_weights[layer].GetRow(); j++) {
        teta += LocalGrads[j] * m_weights[layer][j][i];
      }
      NewGrads.push_back(teta *
                         ActicateFunctionDerivative(m_neurons[layer][i][0]));
    }
    LocalGrads = NewGrads;
  }
}

/**
 * @brief Изменение весов пропорциоеально разнице ожидаемых и актуальных
 * значений
 * @param ExpectedValues ожидаемый результат feedForward для входных значений
 */
auto MatrixNetwork::BackPropagation(std::vector<double> &ExpectedValues)
    -> void {
  std::vector<double> localGrads;
  // #pragma omp parallel for
  for (int i = m_weights.size() - 1; i >= 0; i--) {
    GetLocalGrads(localGrads, ExpectedValues, i + 1);

    for (size_t k = 0; k < m_weights[i].GetColumn(); k++) {
      for (size_t j = 0; j < m_weights[i].GetRow(); j++) {
        m_weights[i][j][k] += step * localGrads[j] * m_neurons[i][k][0];
      }
    }
  }
}

void MatrixNetwork::SaveWeights(std::string filename) {
  std::fstream file;
  file.open(filename, file.out);
  file << "Network weights" << std::endl;
  for (int i = 0; i < m_topology.size(); i++) {
    file << m_topology[i] << ' ';
  }
  file << '\n';
  for (size_t layer = 0; layer < m_weights.size(); layer++) {
    for (size_t i = 0; i < m_weights[layer].GetRow(); i++) {
      for (size_t j = 0; j < m_weights[layer].GetColumn(); j++) {
        file << m_weights[layer][i][j] << std::endl;
      }
    }
  }
  file.close();
}

auto MatrixNetwork::CheckTopology(const std::vector<size_t> &topology) -> bool {
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

auto MatrixNetwork::GetWeights(std::string filename) -> bool {
  setlocale(LC_NUMERIC, "C");
  std::fstream file;
  file.open(filename, file.in);
  if (!file.is_open()) return false;

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
    for (size_t layer = 0; layer < m_weights.size(); layer++) {
      for (size_t i = 0; i < m_weights[layer].GetRow(); i++) {
        for (size_t j = 0; j < m_weights[layer].GetColumn(); j++) {
          std::getline(file, num, '\n');
          double number = std::stod(num);
          m_weights[layer][i][j] = number;
        }
      }
    }
  } else {
    return false;
  }
  file.close();
  return true;
}

void MatrixNetwork::ShowResult() {
  std::cout << "result matrix:\n";
  m_neurons.back().show();
}

auto MatrixNetwork::GetResult() -> size_t {
  size_t res = 0;
  double max = m_neurons.back()[0][0];
  for (int i = 0; i < m_neurons.back().GetRow(); i++) {
    if (max < m_neurons.back()[i][0]) {
      max = m_neurons.back()[i][0];
      res = i;
    }
  }
  return res;
}

auto MatrixNetwork::GetResultVector() -> const std::vector<double> {
  std::vector<double> res(m_topology.back());
  for (int i = 0; i < res.size(); i++) {
    res[i] = m_neurons.back()[i][0];
  }
  return res;
}
}  // namespace s21
