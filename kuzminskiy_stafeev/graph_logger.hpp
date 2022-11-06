#pragma once
#include <fstream>
#include "config.hpp"
#include "graph.hpp"

class Logger {
 public:
  static Logger& get_logger() {
    static Logger instance;
    return instance;
  }

  void log(const std::string& str);

 private:
  Logger() { output_fstream.open(config::kLogFilePath, std::ios::app); }
  std::ofstream output_fstream;
};
