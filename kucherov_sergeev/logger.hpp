#pragma once

#include <fstream>
#include <iostream>
#include "config.hpp"

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    logger.log_file.open(config::kLogFilePath, std::ios::out);

    if (logger.log_file.fail()) {
      throw std::runtime_error("Failed to open " + config::kLogFilePath +
                               " for logging");
    }

    return logger;
  };

  void log(const std::string& string);

 private:
  void log_to_file(const std::string& string);

  std::ofstream log_file;
};
}  // namespace uni_course_cpp
