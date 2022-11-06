#pragma once
#include <fstream>
#include "config.hpp"
#include "graph.hpp"

class Logger {
 public:
  static Logger& get_logger();
  void log(const std::string& str);

 private:
  Logger();
  ~Logger() = default;
  std::ofstream output_fstream_;
};
