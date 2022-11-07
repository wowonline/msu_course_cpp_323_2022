#pragma once
#include <string>

namespace config {
inline constexpr const char* kTempDirectoryPath = "./temp/";
inline constexpr const char* kLogFilename = "log.txt";
inline const std::string kLogFilePath =
    kTempDirectoryPath + std::string(kLogFilename);
}  // namespace config
