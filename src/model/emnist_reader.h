/**
 * @file emnist_reader.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#ifndef S21_MLP_EMNIST_READER_H
#define S21_MLP_EMNIST_READER_H

#include <fstream>
#include <string>
#include <vector>

namespace s21 {

struct CaseData {
  /**
   * @struct CaseData
   * @brief Holds data from file
   */

  std::vector<double> m_input;
  int m_result;

  CaseData() = default;
  ~CaseData() = default;
  CaseData(const std::vector<double>& input, int res);
};

class EmnistReader {
  /**
   * @class EmnistReader
   * @brief Reader for emnist-letters files with *csv extension
   * @param whatFile says is it test or train file
   */

 public:
  auto OpenFile(std::string whatFile) -> void;
  auto ReadLine() -> std::vector<double>;
  auto GetFileSize() -> int;
  auto IsOpen() -> bool;
  auto CloseFile() -> void;
  auto GetCaseData() -> CaseData;

  EmnistReader() : m_file(nullptr), m_file_size(0) {}

 private:
  std::fstream m_file;
  int m_file_size;
};
}  // namespace s21
#endif
