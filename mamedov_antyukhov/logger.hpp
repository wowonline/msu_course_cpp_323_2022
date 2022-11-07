#include <string>
namespace uni_course_cpp {
std::string get_current_date_time();
class Logger {
    public:
    static Logger* get_logger();
    void log(const std::string& string) const;

    private:
    static Logger* instance_;
    Logger(){};
};
}