#include <chrono>

#include "emnist_reader.h"
#include "network.h"

namespace s21 {

auto Metrics::Calculate(int tp, int fp, int tn, int fn, int dataSize) -> void {
  accuracy = static_cast<double>(accuracy) / dataSize;
  precision = static_cast<double>(tp) / static_cast<double>(tp + fp);
  recall = static_cast<double>(tp) / static_cast<double>(tp + fn);
  f_measure = 2 * (precision * recall) / (precision + recall);
}

auto NetworkInterface::TestNet(const std::string& fileName,
                               const double datasetUsage) -> Metrics {
  EmnistReader reader;
  Metrics metrics;
  reader.OpenFile(fileName);
  const int fileSize = reader.GetFileSize();

  int dataSize = static_cast<int>(fileSize * datasetUsage);
  int tp = 0, fp = 0, tn = 0, fn = 0;
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < dataSize; i++) {
    s21::CaseData data = reader.GetCaseData();
    if (reader.IsOpen()) {
      this->FeedInitValues(data.m_input);
      this->FeedForward();
      std::vector<double> res = this->GetResultVector();
      int answer = this->GetResult();
      if (data.m_result == answer + 1) metrics.accuracy++;
      for (int j = 0; j < 26; j++) {
        if (j == answer) {
          if (res[j] > 0.5)
            tp++;
          else
            fp++;
        } else {
          if (res[j] > 0.5)
            fn++;
          else
            tn++;
        }
      }
    }
  }

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(stop - start);
  metrics.ed_time = duration.count();

  metrics.Calculate(tp, fp, tn, fn, dataSize);
  reader.CloseFile();
  return metrics;
}

auto NetworkInterface::Train(const std::string& fileName, const size_t epochs)
    -> std::vector<double> {
  EmnistReader reader;
  std::vector<double> errors;
  for (size_t i = 0; i < epochs; i++) {
    int accuracy = 0;
    reader.OpenFile(fileName);
    const int dataSize = reader.GetFileSize();
    for (int j = 0; j < dataSize; j++) {
      std::vector<double> expected_vals(m_topology.back());
      s21::CaseData data = reader.GetCaseData();
      expected_vals[data.m_result - 1] = 1;
      this->FeedInitValues(data.m_input);
      this->FeedForward();
      if (static_cast<size_t>(data.m_result - 1) == this->GetResult()) {
        accuracy++;
      }
      this->BackPropagation(expected_vals);
    }
    accuracy =
        (static_cast<double>(accuracy) / static_cast<double>(dataSize)) * 100.0;
    errors.push_back(100.0 - accuracy);
  }
  reader.CloseFile();

  return errors;
}

auto NetworkInterface::CrossValidation(const std::string& fileName,
                                       const size_t k) -> std::vector<double> {
  EmnistReader reader;
  reader.OpenFile(fileName);
  const int fileSize = reader.GetFileSize();

  std::vector<s21::CaseData> testDataInput;
  std::vector<double> accuracyVals;

  std::vector<double> expectedVals(m_topology.back());
  s21::CaseData data;

  int testDataSize = fileSize / k;
  /* индексы строки, обозначающие начало и конец данных для теста*/
  int getTest = 0, endTest = testDataSize;

  for (int i = 0; i < k; i++) {
    int accuracy = 0;
    for (int iterator = 0; iterator < fileSize; iterator++) {
      if (iterator >= getTest && iterator < endTest) {
        /// сбор данных для теста
        testDataInput.push_back(reader.GetCaseData());
      } else {
        /// обучение
        data = reader.GetCaseData();
        expectedVals[data.m_result - 1] = 1;
        this->FeedInitValues(data.m_input);
        this->FeedForward();
        this->BackPropagation(expectedVals);
        expectedVals[data.m_result - 1] = 0;
      }
    }
    /// тест
    for (int j = 0; j < testDataInput.size(); j++) {
      this->FeedInitValues(testDataInput[j].m_input);
      this->FeedForward();
      if (this->GetResult() == testDataInput[j].m_result - 1) accuracy++;
    }

    getTest += testDataSize;
    endTest += testDataSize;

    testDataInput.clear();
    accuracyVals.push_back(static_cast<double>(accuracy) / testDataSize * 100);
    if (i != k - 1) reader.OpenFile(fileName);
  }

  reader.CloseFile();
  return accuracyVals;
}

}  // namespace s21
