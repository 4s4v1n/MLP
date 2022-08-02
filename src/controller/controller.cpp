/**
 * @file controller.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "controller.h"

namespace s21 {

Controller::~Controller() { delete m_net; }

auto Controller::SetNet(s21::NetworkType netType, int hiddenLayers) -> void {
  if (netType != m_netType || hiddenLayers != m_hiddenLayers) {
    delete m_net;
    if (netType == s21::NetworkType::MATRIX) {
      m_net = new MatrixNetwork;
    } else {
      m_net = new GraphNetwork;
    }
    m_netType = netType;
    m_hiddenLayers = hiddenLayers;
    m_net->SetLayers(m_net->GetLayersVector(m_hiddenLayers));
  }
}

auto Controller::GetNet() const -> NetworkInterface* { return m_net; }

auto Controller::FeedForward() -> void { m_net->FeedForward(); }

auto Controller::FeedInitValues(const std::vector<double>& values) -> void {
  m_net->FeedInitValues(values);
}

auto Controller::GetResult() -> size_t { return m_net->GetResult(); }

auto Controller::SaveWeights(const std::string& fileName) -> void {
  m_net->SaveWeights(fileName);
}

auto Controller::LoadWeights(const std::string& fileName) -> void {
  m_net->GetWeights(fileName);  // поменять в сети имя метода
}

auto Controller::GetMetrics(const std::string& fileName,
                            const double datasetUsage) -> Metrics {
  return m_net->TestNet(fileName, datasetUsage);
}

auto Controller::Train(const std::string& fileName, const size_t epochs)
    -> std::vector<double> {
  return m_net->Train(fileName, epochs);
}

auto Controller::CrossValidation(const std::string& fileName, const size_t k)
    -> std::vector<double> {
  return m_net->CrossValidation(fileName, k);
}

}  // namespace s21
