#pragma once
#include <fstream>
#include <string>

class Logger {
 public:
  static Logger& get_logger();
  void log(const std::string& str);

 private:
  Logger();
  ~Logger() = default;
  std::ofstream output_fstream_;
};
