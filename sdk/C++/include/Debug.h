#include <iostream>

// 日志级别
enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

// Debug 类
class Debug {
public:
    Debug(LogLevel level) : level(level), prefixPrinted(false) {
        file.open("./log/" + getCurrentTime() + ".txt", std::ios::app);
    }

    template<typename T>
    Debug& operator<<(const T& value) {
        if (!prefixPrinted) {
            if (level == LogLevel::INFO) {
                file << "[INFO] ";
            } else if (level == LogLevel::WARNING) {
                file << "[WARNING] ";
            } else if (level == LogLevel::ERROR) {
                file << "[ERROR] ";
            }
            prefixPrinted = true;
        }
        file << value;
        return *this;
    }

    Debug& operator<<(std::ostream& (*func)(std::ostream&)) {
        file << func;
        if (func == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
            prefixPrinted = false; // 如果是插入 endl，表示链式输出结束，重置 printedPrefix
        }
        return *this;
    }



private:
    LogLevel level;
    std::ofstream file;
    bool prefixPrinted;
};