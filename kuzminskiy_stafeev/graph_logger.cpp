#include "graph_logger.hpp"
#include <iostream>
#include "config.hpp"

Logger::Logger()
    : output_fstream_(std::ofstream(config::kLogFilePath, std::ios::app)) {}

Logger& Logger::get_logger() {
  static Logger instance;
  return instance;
}

void Logger::log(const std::string& str) {
  std::cout << str << std::endl;
  output_fstream_ << str.c_str() << std::endl;
}
