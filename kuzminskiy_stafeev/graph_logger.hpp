#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "config.hpp"
#include "graph.hpp"
#pragma once

class Logger {
 public:
  static Logger* get_logger() {
    if (NULL == _instance) {
      _instance = new Logger();
      _instance->ofs.open(config::kLogFilePath, std::ios::app);
    }

    return _instance;
  }

  void log(const std::string& str);

 private:
  Logger(void) {}
  virtual ~Logger(void) {}
  static Logger* _instance;
  std::ofstream ofs;

  class CGarbo {
   public:
    ~CGarbo() {
      if (Logger::_instance) {
        Logger::_instance->ofs.close();
        delete Logger::_instance;
        Logger::_instance = NULL;
      }
    }
  };

  static CGarbo Garbo;
};
