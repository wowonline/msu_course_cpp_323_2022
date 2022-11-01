#include "graph_logger.hpp"

Logger* Logger::_instance = NULL;

void Logger::log(const std::string& str) {
  ofs << str.c_str() << std::endl;
}
