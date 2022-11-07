#include <string>

namespace config {

inline constexpr const char* kTempDirectoryPath = "temp/";
inline constexpr const char* kLogFilename = "log.txt";
inline const std::string kLogFilePath =
    static_cast<std::string>(kTempDirectoryPath) +
    static_cast<std::string>(kLogFilename);

}  // namespace config