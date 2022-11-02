#pragma once

#include "config.hpp"

#include <fstream>
#include <iostream>

namespace uni_course_cpp {
std::string get_current_date_time();
class Logger {
 public:
  static Logger& get_logger() {
    static Logger instance;
    return instance;
  }
  void log(const std::string string) const {
    const auto string_with_date_time = get_current_date_time() + " " + string;
    std::cout << string_with_date_time << std::endl;
    log_to_file(string_with_date_time);
  }

 private:
  void log_to_file(const std::string& content) const {
    std::ofstream log_file(config::kLogFilePath, std::ios_base::app);
    log_file << content << std::endl;
  }
};
}  // namespace uni_course_cpp