#include "Debug.h"
#include "globals.h"

Debug::Debug() {
    outFile.open("out.txt");
    if (!outFile) {
        std::cout << "open file error" << endl;
        exit(1);
    }
}

Debug::~Debug() {

}

std::ostream& operator<<(std::ostream& os, const Debug& debug) {
    return os;
}

void Debug::log(std::string str) {
    outFile << "Info: " << str << std::endl;
}

void Debug::warn(std::string str) {
    outFile << "Warning: " << str << std::endl;
}

void Debug::error(std::string str) {
    outFile << "Error: " << str << std::endl;
}
