/**
 * @file emnist_reader.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "emnist_reader.h"

#include <iostream>

namespace s21 {

CaseData::CaseData(const std::vector<double>& input, int res)
    : m_input(input), m_result(res) {}

auto EmnistReader::OpenFile(std::string fileName) -> void {
  m_file.close();
  m_file_size = 0;
  m_file.open(fileName);
  if (!m_file.is_open()) {
    throw std::invalid_argument("DAUN: File " + fileName + " not found");
  }
  char symb;
  while (!m_file.eof()) {
    m_file.get(symb);
    if (!m_file.eof() && symb == '\n') {
      m_file_size++;
    }
  }
  m_file.close();
  m_file.open(fileName);
}

auto EmnistReader::GetFileSize() -> int { return m_file_size; }

auto EmnistReader::GetCaseData() -> CaseData {
  std::vector<double> input = ReadLine();
  if (this->IsOpen()) {
    int res = input.back();
    input.pop_back();
    return CaseData(input, res);
  } else {
    return CaseData(std::vector<double>(), 0);
  }
}

auto EmnistReader::ReadLine() -> std::vector<double> {
  if (!m_file.is_open()) {
    throw std::invalid_argument("file is not open - cannot read");
  }
  std::string temp;
  std::vector<double> out;

  char c = 0;
  double result;
  bool firstNumber = true;
  while (c != '\n' && !m_file.eof()) {
    m_file.get(c);
    if (c != ',' && c != '\n' && !m_file.eof()) {
      temp += c;
    } else if (!m_file.eof()) {
      double res = std::stod(temp);
      if (firstNumber) {
        result = res;
        firstNumber = false;
      } else {
        out.push_back(res / 255.0);
      }
      temp = "";
    }
  }
  out.push_back(result);

  if (m_file.eof()) {
    m_file.close();
  }
  return out;
}

auto EmnistReader::IsOpen() -> bool { return m_file.is_open(); }

auto EmnistReader::CloseFile() -> void { m_file.close(); }
}  // namespace s21
