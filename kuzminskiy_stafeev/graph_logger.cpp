#include "graph_logger.hpp"

void Logger::log(const std::string& str) {
  output_fstream << str.c_str() << std::endl;
}
